#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Tasmota ESP32 빌드 스크립트 (PlatformIO 래퍼)

브릿지/오디오(idf.py)와 달리 Tasmota는 Arduino+PlatformIO 전용입니다.
이 스크립트는 VS Code 없이 터미널에서 동일하게 빌드합니다.

사용:
  python build_tasmota.py
  python build_tasmota.py --env tasmota32
  python build_tasmota.py --clean
  python build_tasmota.py --upload COM5
  python build_tasmota.py --upload-only COM10
  python build_tasmota.py --upload-only COM10 --baud 460800
"""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
DEFAULT_ENV = "tasmota32"
DEFAULT_UPLOAD_BAUD = "921600"
FIRMWARE = ROOT / ".pio" / "build" / DEFAULT_ENV / "firmware.bin"


def find_platformio() -> Path:
    """PlatformIO CLI 경로 탐색"""
    candidates = [
        Path(os.environ.get("PLATFORMIO_CORE_DIR", "")) / "penv" / "Scripts" / "platformio.exe",
        Path.home() / ".platformio" / "penv" / "Scripts" / "platformio.exe",
        Path.home() / ".platformio" / "penv" / "Scripts" / "pio.exe",
    ]
    for exe in candidates:
        if exe.is_file():
            return exe
    found = shutil.which("platformio") or shutil.which("pio")
    if found:
        return Path(found)
    print("PlatformIO를 찾을 수 없습니다.")
    print("  VS Code 확장 'PlatformIO IDE' 설치 후 다시 시도하거나")
    print("  pip install platformio")
    sys.exit(1)


def run(cmd: list[str], env: dict[str, str] | None = None) -> int:
    print(f"\n>>> {' '.join(cmd)}\n")
    result = subprocess.run(
        cmd,
        cwd=ROOT,
        env=env,
        encoding="utf-8",
        errors="replace",
    )
    return result.returncode


def _path_is_junction(path: Path) -> bool:
    """Windows junction 여부 (Python 3.11 이하 호환)"""
    if hasattr(path, "is_junction"):
        try:
            return path.is_junction()
        except OSError:
            return False
    if os.name != "nt" or not path.exists():
        return False
    try:
        import stat
        return bool(path.lstat().st_file_attributes & stat.FILE_ATTRIBUTE_REPARSE_POINT)
    except (OSError, AttributeError):
        return False


def fix_toolchain_layout() -> bool:
    """
    toolchain-xtensa-esp-elf 레이아웃 보정.
    - bin/libexec: xtensa-esp-elf/ 아래에만 있으면 junction
    - lib: nested xtensa-esp-elf/xtensa-esp-elf/lib (nano.specs) 로 junction
    - include: 이중 xtensa-esp-elf/ 중첩 시 sys-include 경로 junction
    """
    pkg = Path.home() / ".platformio" / "packages" / "toolchain-xtensa-esp-elf"
    inner = pkg / "xtensa-esp-elf"

    def _same_target(link: Path, target: Path) -> bool:
        try:
            return link.exists() and link.resolve() == target.resolve()
        except OSError:
            return False

    def _has_gcc_lib(link: Path) -> bool:
        """lib/gcc 는 컴파일러 헤더·specs 경로 — 삭제·junction 교체 금지"""
        return (link / "gcc").is_dir()

    def _remove_link(link: Path) -> None:
        """junction 또는 빈 폴더만 제거 (lib/gcc 보존)"""
        if not link.exists():
            return
        try:
            if _path_is_junction(link) or link.is_symlink():
                link.rmdir()
                return
            if link.is_dir():
                if _has_gcc_lib(link):
                    return
                if not any(link.iterdir()):
                    link.rmdir()
        except OSError as exc:
            print(f"  [툴체인] {link.name} 제거 실패: {exc}")

    def _copy_linker_specs(src_lib: Path, dst_lib: Path) -> None:
        """nano.specs·링크 아카이브 복사 — lib/gcc 는 건드리지 않음"""
        if not src_lib.is_dir() or not dst_lib.is_dir():
            return
        if _path_is_junction(dst_lib) or dst_lib.is_symlink():
            return
        for name in (
            "nano.specs", "nosys.specs", "picolibc.specs", "default.specs",
            "picolibcpp.specs", "board.elf.specs", "sim.elf.specs",
            "sys.openocd.specs", "sys.qemu.specs",
            "crt0.o", "libc.a", "libc_nano.a", "libg.a", "libg_nano.a",
            "libm.a", "libm_nano.a", "libnosys.a", "libpthread_stubs.a",
            "libstdc++.a", "libstdc++exp.a", "libsupc++.a", "libgloss.a",
        ):
            src = src_lib / name
            dst = dst_lib / name
            if src.is_file() and not dst.is_file():
                shutil.copy2(src, dst)
                print(f"[툴체인] spec/lib 복사: {name} -> {dst_lib.name}")

    def _junction(link: Path, target: Path, *, force: bool = False) -> bool:
        if not target.is_dir():
            return False
        if link.exists():
            if not force and _same_target(link, target):
                return True
            _remove_link(link)
            if link.exists():
                print(f"  [툴체인] {link} 정리 실패 — junction 생략")
                return False
        print(f"[툴체인] junction: {link} -> {target}")
        rc = subprocess.run(
            ["cmd", "/c", "mklink", "/J", str(link), str(target)],
            capture_output=True,
            text=True,
        )
        if rc.returncode != 0:
            print(f"  실패: {rc.stderr.strip() or rc.stdout.strip()}")
            return False
        return True

    def _find_spec_lib() -> Path | None:
        """nano.specs 가 있는 lib 디렉터리 탐색"""
        for candidate in (
            inner / "lib",
            inner / "xtensa-esp-elf" / "lib",
        ):
            if (candidate / "nano.specs").is_file():
                return candidate
        return None

    def _fix_lib_junctions() -> None:
        """pkg/lib(gcc) + spec lib(nano.specs) 병합"""
        spec_lib = _find_spec_lib()
        if spec_lib is None:
            return

        pkg_lib = pkg / "lib"
        inner_lib = inner / "lib"

        # gcc 전용 pkg/lib 에 specs/아카이브 보충
        if pkg_lib.is_dir() and not _path_is_junction(pkg_lib):
            _copy_linker_specs(spec_lib, pkg_lib)

        # inner/lib 에 gcc 있고 specs 없을 때만 보충
        if inner_lib.is_dir() and not _path_is_junction(inner_lib) and _has_gcc_lib(inner_lib):
            _copy_linker_specs(spec_lib, inner_lib)

        # bin/libexec 만 junction (lib 는 gcc+specs 병합 유지)
        inner_gpp = inner / "bin" / "xtensa-esp32-elf-g++.exe"
        pkg_gpp = pkg / "bin" / "xtensa-esp32-elf-g++.exe"
        if not pkg_gpp.is_file() and not (pkg / "bin" / "xtensa-esp-elf-g++.exe").is_file():
            if inner_gpp.is_file() or (inner / "bin" / "xtensa-esp-elf-g++.exe").is_file():
                for name in ("bin", "libexec"):
                    if not (pkg / name).exists():
                        _junction(pkg / name, inner / name)

    gpp = pkg / "bin" / "xtensa-esp32-elf-g++.exe"
    if not gpp.is_file():
        gpp = pkg / "bin" / "xtensa-esp-elf-g++.exe"
    inner_gpp = inner / "bin" / "xtensa-esp32-elf-g++.exe"
    if not inner_gpp.is_file():
        inner_gpp = inner / "bin" / "xtensa-esp-elf-g++.exe"
    if not gpp.is_file() and inner_gpp.is_file():
        for name in ("bin", "libexec"):
            _junction(pkg / name, inner / name)

    _fix_lib_junctions()

    # stdint.h: xtensa-esp-elf/include 또는 이중 중첩 xtensa-esp-elf/xtensa-esp-elf/include
    stdint_candidates = [
        inner / "include" / "stdint.h",
        inner / "xtensa-esp-elf" / "include" / "stdint.h",
        inner / "picolibc" / "include" / "stdint.h",
    ]
    gcc_include = pkg / "lib" / "gcc" / "xtensa-esp-elf" / "15.1.0" / "include"
    if not gcc_include.is_dir():
        gcc_include = inner / "lib" / "gcc" / "xtensa-esp-elf" / "15.1.0" / "include"
    if not gcc_include.is_dir():
        print("[경고] lib/gcc 없음 - 툴체인 손상. 재설치:")
        print("  pio pkg uninstall -e tasmota32 toolchain-xtensa-esp-elf")
        print("  pio run -e tasmota32  (자동 재다운로드)")
        return False

    if not any(p.is_file() for p in stdint_candidates):
        print(f"[경고] stdint.h 없음 - 툴체인 재설치 필요: {pkg}")
        return False

    if not (inner / "include" / "stdint.h").is_file():
        for nested in (inner / "xtensa-esp-elf" / "include", inner / "picolibc" / "include"):
            if (nested / "stdint.h").is_file():
                _junction(inner / "include", nested)
                break

    # gcc 가 lib/../xtensa-esp-elf/bin/as 를 찾음 (prefixed as.exe 만 있을 때)
    inner_bin = inner / "bin"
    if inner_bin.is_dir():
        for tool in ("as", "ld"):
            plain = inner_bin / f"{tool}.exe"
            prefixed = inner_bin / f"xtensa-esp-elf-{tool}.exe"
            if prefixed.is_file() and not plain.is_file():
                shutil.copy2(prefixed, plain)
                print(f"[툴체인] {tool}.exe <- xtensa-esp-elf-{tool}.exe")

    if gpp.is_file() and (inner / "include" / "stdint.h").is_file():
        return True
    if inner_gpp.is_file() and (inner / "include" / "stdint.h").is_file():
        return True

    print(f"[경고] 툴체인 불완전: {pkg}")
    return False


def toolchain_bin_paths() -> list[str]:
    """컴파일러 검색 경로"""
    pio_home = Path.home() / ".platformio" / "packages"
    paths = [
        pio_home / "toolchain-xtensa-esp-elf" / "bin",
        pio_home / "toolchain-xtensa-esp-elf" / "xtensa-esp-elf" / "bin",
        pio_home / "toolchain-xtensa-esp32" / "bin",
    ]
    return [str(p) for p in paths if p.is_dir()]


def make_env() -> dict[str, str]:
    """PowerShell ESP-IDF export.ps1 / PlatformIO 툴체인 경로 충돌 제거"""
    env = os.environ.copy()
    # 프로필의 export.ps1 (IDF 5.5 + .espressif) 이 Tasmota PlatformIO (.platformio) 와 충돌
    for key in (
        "IDF_PATH",
        "IDF_PYTHON_ENV_PATH",
        "OPENOCD_SCRIPTS",
        "IDF_TOOLS_PATH",
        "ESP_IDF_VERSION",
        "ESP_ROM_ELF_DIR",
    ):
        env.pop(key, None)
    pio_home = Path.home() / ".platformio"
    tc_bin = pio_home / "packages" / "toolchain-xtensa-esp-elf" / "bin"
    extra = [str(pio_home / "penv" / "Scripts")]
    if tc_bin.is_dir():
        extra.append(str(tc_bin))
    extra.extend(toolchain_bin_paths())
    env["PATH"] = os.pathsep.join(extra) + os.pathsep + env.get("PATH", "")
    return env


def find_esptool_cmd() -> list[str] | None:
    """esptool 실행 명령 (Windows: python esptool.py)"""
    pio_python = Path.home() / ".platformio" / "penv" / "Scripts" / "python.exe"
    esptool_py = Path.home() / ".platformio" / "packages" / "tool-esptoolpy" / "esptool.py"
    if pio_python.is_file() and esptool_py.is_file():
        return [str(pio_python), str(esptool_py)]

    esptool_exe = Path.home() / ".platformio" / "packages" / "tool-esptoolpy" / "esptool.exe"
    if esptool_exe.is_file():
        return [str(esptool_exe)]

    found = shutil.which("esptool")
    if found:
        return [found]
    return None


def find_safeboot_bin() -> Path | None:
    """tasmota32-safeboot.bin 탐색"""
    framework = Path.home() / ".platformio" / "packages" / "framework-arduinoespressif32"
    candidates = [
        ROOT / "variants" / "tasmota" / "tasmota32-safeboot.bin",
        framework / "variants" / "tasmota" / "tasmota32-safeboot.bin",
    ]
    for path in candidates:
        if path.is_file():
            return path
    return None


def upload_firmware_only(port: str, env_name: str, shell_env: dict[str, str], baud: str) -> int:
    """재빌드 없이 esptool로 플래시 (Tasmota ESP32 다중 파티션)"""
    build = ROOT / ".pio" / "build" / env_name
    esptool_cmd = find_esptool_cmd()
    if esptool_cmd is None:
        print("[오류] esptool을 찾을 수 없습니다.")
        return 1

    factory = build / "firmware.factory.bin"
    firmware = build / "firmware.bin"
    if not factory.is_file() and not firmware.is_file():
        print(f"[오류] 빌드 산출물 없음: {build}")
        print("  먼저: python build_tasmota.py")
        return 1

    flash_args = esptool_cmd + [
        "--chip", "esp32",
        "--port", port,
        "--baud", baud,
        "--before", "default-reset",
        "--after", "hard-reset",
        "write-flash", "-z", "--no-progress",
        "--flash-mode", "dio",
        "--flash-freq", "40m",
        "--flash-size", "4MB",
    ]

    if factory.is_file():
        # post_esp32.py 가 만든 통합 이미지 - offset 0
        flash_args += ["0x0", str(factory)]
        print(f"[플래시] {factory.name} → {port}")
    else:
        framework = Path.home() / ".platformio" / "packages" / "framework-arduinoespressif32"
        boot_app0 = framework / "tools" / "partitions" / "boot_app0.bin"
        bootloader = build / "bootloader.bin"
        partitions = build / "partitions.bin"
        safeboot = find_safeboot_bin()
        missing = [p for p in (bootloader, partitions, boot_app0, safeboot) if not p or not p.is_file()]
        if missing:
            print("[오류] factory.bin 없고 개별 이미지도 불완전합니다.")
            print("  python build_tasmota.py  (한 번 전체 빌드 필요)")
            for p in missing:
                print(f"  없음: {p}")
            return 1
        flash_args += [
            "0x1000", str(bootloader),
            "0x8000", str(partitions),
            "0xe000", str(boot_app0),
            "0x10000", str(safeboot),
            "0xe0000", str(firmware),
        ]
        print(f"[플래시] 다중 파티션 → {port}")

    upload_env = shell_env.copy()
    upload_env["PYTHONIOENCODING"] = "utf-8"
    upload_env["PYTHONUTF8"] = "1"

    return run(flash_args, upload_env)


def main() -> int:
    parser = argparse.ArgumentParser(description="Tasmota PlatformIO 빌드")
    parser.add_argument("--env", default=DEFAULT_ENV, help=f"빌드 환경 (기본: {DEFAULT_ENV})")
    parser.add_argument("--clean", action="store_true", help="빌드 캐시 삭제 후 빌드")
    parser.add_argument("--upload", metavar="COM", help="빌드 후 플래시 (예: COM5)")
    parser.add_argument("--upload-only", metavar="COM", help="기존 firmware.bin만 플래시 (재빌드 없음)")
    parser.add_argument(
        "--baud",
        default=DEFAULT_UPLOAD_BAUD,
        help=f"esptool 업로드 속도 (기본: {DEFAULT_UPLOAD_BAUD}, CH340 불안정 시 460800)",
    )
    parser.add_argument("--update", action="store_true", help="PlatformIO 패키지 업데이트 후 빌드")
    args = parser.parse_args()

    pio = find_platformio()
    fix_toolchain_layout()
    env = make_env()
    base = [str(pio)]

    # tasmota.ino → .cpp 변환본 — 삭제 시 PlatformIO가 재생성 못 하면 Nothing to build 발생
    ino_cpp = ROOT / "tasmota" / "tasmota.ino.cpp"
    ino_cpp_bak = ROOT / "tasmota" / "tasmota.ino.cpp.bak"
    if args.clean and ino_cpp.is_file():
        try:
            ino_cpp.unlink()
            print(f"[빌드] clean: {ino_cpp.name} 삭제")
        except OSError as exc:
            print(f"[오류] {ino_cpp} 삭제 실패(다른 프로그램이 사용 중): {exc}")
            print("  VS Code/Cursor에서 tasmota.ino.cpp 탭을 닫고 재시도하세요.")
            return 1
    elif not ino_cpp.is_file() and ino_cpp_bak.is_file():
        shutil.copy2(ino_cpp_bak, ino_cpp)
        print(f"[빌드] {ino_cpp.name} 없음 → .bak에서 복원")

    if args.update:
        if run(base + ["pkg", "update"], env) != 0:
            return 1

    if args.clean:
        if run(base + ["run", "-e", args.env, "-t", "clean"], env) != 0:
            return 1
        for path in (ROOT / ".pio" / "build" / args.env, ROOT / ".cache"):
            if path.exists():
                shutil.rmtree(path, ignore_errors=True)
                print(f"삭제: {path}")

    if args.upload_only:
        return upload_firmware_only(args.upload_only, args.env, env, args.baud)

    cmd = base + ["run", "-e", args.env]
    if args.upload:
        cmd += ["-t", "upload", "--upload-port", args.upload]

    rc = run(cmd, env)
    if rc != 0:
        print("\n[실패] 흔한 원인:")
        print("  1) Cursor/VS Code·PuTTY·시리얼 모니터 완전 종료 후 재시도 (DLL 잠금 WinError 32)")
        print("  2) 'Tool Manager: Installing file://...' 는 재다운로드가 아니라 로컬->packages 등록, 끝까지 기다리기")
        print("  3) junction: cmd /c mklink /J %USERPROFILE%\\.platformio\\packages\\toolchain-xtensa-esp-elf\\bin %USERPROFILE%\\.platformio\\packages\\toolchain-xtensa-esp-elf\\xtensa-esp-elf\\bin")
        print("  ※ --update --clean 은 툴체인 재설치를 유발하므로 툴체인 오류 시 사용 금지")
        return rc

    fw = ROOT / ".pio" / "build" / args.env / "firmware.bin"
    if fw.is_file():
        print(f"\n[성공] {fw}")
        print(f"       크기: {fw.stat().st_size // 1024} KB")
        if ino_cpp.is_file():
            shutil.copy2(ino_cpp, ino_cpp_bak)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
