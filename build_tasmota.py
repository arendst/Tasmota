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


def fix_toolchain_layout() -> bool:
    """
    toolchain-xtensa-esp-elf 가 xtensa-esp-elf/ 아래에만 풀린 경우 bin·lib·libexec junction 보정.
    packages/ 루트에 bin/ 이 있으면(정상 설치) 아무 것도 하지 않음.
    include junction은 picolibc.specs 경로를 깨므로 만들지 않음.
    """
    pkg = Path.home() / ".platformio" / "packages" / "toolchain-xtensa-esp-elf"
    if (pkg / "bin" / "xtensa-esp32-elf-g++.exe").is_file():
        return True

    inner = pkg / "xtensa-esp-elf"
    if not (inner / "bin" / "xtensa-esp32-elf-g++.exe").is_file():
        print(f"[경고] 툴체인 없음: {pkg}")
        return False

    ok = True
    for name in ("bin", "lib", "libexec"):
        nested = inner / name
        expected = pkg / name
        if not nested.is_dir() or expected.exists():
            continue
        print(f"[툴체인] junction: {name}")
        rc = subprocess.run(
            ["cmd", "/c", "mklink", "/J", str(expected), str(nested)],
            capture_output=True,
            text=True,
        )
        if rc.returncode != 0:
            print(f"  실패: {rc.stderr.strip()}")
            ok = False
    return ok


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
    """ESP-IDF 자동 활성화와 PATH 충돌 완화"""
    env = os.environ.copy()
    for key in ("IDF_PATH", "IDF_PYTHON_ENV_PATH", "OPENOCD_SCRIPTS"):
        env.pop(key, None)
    pio_home = Path.home() / ".platformio"
    extra = [str(pio_home / "penv" / "Scripts")] + toolchain_bin_paths()
    env["PATH"] = os.pathsep.join(extra) + os.pathsep + env.get("PATH", "")
    return env


def main() -> int:
    parser = argparse.ArgumentParser(description="Tasmota PlatformIO 빌드")
    parser.add_argument("--env", default=DEFAULT_ENV, help=f"빌드 환경 (기본: {DEFAULT_ENV})")
    parser.add_argument("--clean", action="store_true", help="빌드 캐시 삭제 후 빌드")
    parser.add_argument("--upload", metavar="COM", help="빌드 후 플래시 (예: COM5)")
    parser.add_argument("--update", action="store_true", help="PlatformIO 패키지 업데이트 후 빌드")
    args = parser.parse_args()

    pio = find_platformio()
    fix_toolchain_layout()
    env = make_env()
    base = [str(pio)]

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

    cmd = base + ["run", "-e", args.env]
    if args.upload:
        cmd += ["-t", "upload", "--upload-port", args.upload]

    rc = run(cmd, env)
    if rc != 0:
        print("\n[실패] 흔한 원인:")
        print("  1) python build_tasmota.py --update --clean  (툴체인 bin junction 자동 시도)")
        print("  2) 수동: cmd /c mklink /J %USERPROFILE%\\.platformio\\packages\\toolchain-xtensa-esp-elf\\bin %USERPROFILE%\\.platformio\\packages\\toolchain-xtensa-esp-elf\\xtensa-esp-elf\\bin")
        print("  3) VS Code PlatformIO 확장 재설치")
        return rc

    fw = ROOT / ".pio" / "build" / args.env / "firmware.bin"
    if fw.is_file():
        print(f"\n[성공] {fw}")
        print(f"       크기: {fw.stat().st_size // 1024} KB")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
