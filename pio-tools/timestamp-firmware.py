Import("env")

import re
import shutil
import struct
import pathlib
import subprocess
import tasmotapiolib

_MONTHS = {"Jan":1,"Feb":2,"Mar":3,"Apr":4,"May":5,"Jun":6,
           "Jul":7,"Aug":8,"Sep":9,"Oct":10,"Nov":11,"Dec":12}


def _nm_path(env):
    return env.subst("$CC").replace("-gcc", "-nm")


def _elf_sections(data):
    """ELF32 LE: return (sh_addr, sh_offset, sh_size) for all SHT_PROGBITS sections."""
    e_shoff,     = struct.unpack_from("<I", data, 32)
    e_shentsize, = struct.unpack_from("<H", data, 46)
    e_shnum,     = struct.unpack_from("<H", data, 48)
    sections = []
    for i in range(e_shnum):
        b = e_shoff + i * e_shentsize
        sh_type,   = struct.unpack_from("<I", data, b + 4)
        sh_addr,   = struct.unpack_from("<I", data, b + 12)
        sh_offset, = struct.unpack_from("<I", data, b + 16)
        sh_size,   = struct.unpack_from("<I", data, b + 20)
        if sh_type == 1 and sh_addr and sh_size:  # SHT_PROGBITS with content
            sections.append((sh_addr, sh_offset, sh_size))
    return sections


def _elf_string(data, sections, vma):
    """Read null-terminated ASCII string at VMA from ELF data."""
    for addr, off, size in sections:
        if addr <= vma < addr + size:
            start = off + (vma - addr)
            end   = data.index(b"\x00", start)
            return data[start:end].decode("ascii")
    return None


def _build_timestamp(env, elf_path):
    nm = _nm_path(env)
    r  = subprocess.run([nm, "--defined-only", "--demangle", str(elf_path)],
                        capture_output=True, text=True)
    if r.returncode != 0:
        return None

    date_vma = None
    time_vma = None
    for line in r.stdout.splitlines():
        m = re.match(r"^([0-9a-f]+) \S (.+)$", line)
        if not m:
            continue
        vma, sym = int(m.group(1), 16), m.group(2)
        if sym == "GetBuildDateAndTime()::mdate_P":
            date_vma = vma
        elif sym == "GetBuildDateAndTime()::mtime_P":
            time_vma = vma

    if date_vma is None or time_vma is None:
        return None

    data = elf_path.read_bytes()
    secs = _elf_sections(data)

    date_s = _elf_string(data, secs, date_vma)
    time_s = _elf_string(data, secs, time_vma)
    if not date_s or not time_s:
        return None

    # Parse __DATE__: "May  1 2026" or "Apr 30 2026"
    parts = date_s.split()
    if len(parts) != 3 or parts[0] not in _MONTHS:
        return None
    month, day, year = _MONTHS[parts[0]], int(parts[1]), int(parts[2])

    # Parse __TIME__: "HH:MM:SS"
    if not re.fullmatch(r"\d{2}:\d{2}:\d{2}", time_s):
        return None
    h, mi, s = (int(x) for x in time_s.split(":"))

    return f"{year:04d}-{month:02d}-{day:02d}T{h:02d}-{mi:02d}-{s:02d}"


def _do_copy(source, target, env):
    if not tasmotapiolib.is_env_set(tasmotapiolib.APPEND_TIMESTAMP, env):
        return

    elf_path = pathlib.Path(env.subst("$BUILD_DIR")) / "firmware.elf"
    if not elf_path.is_file():
        print("timestamp-firmware: firmware.elf not found, skipping")
        return

    ts = _build_timestamp(env, elf_path)
    if not ts:
        print("timestamp-firmware: timestamp extraction failed, skipping")
        return

    bin_path = tasmotapiolib.get_final_bin_path(env)
    variant  = bin_path.stem  # e.g. "tasmota-4M"

    out_dir      = bin_path.parent
    factory_path = out_dir / (bin_path.stem + ".factory.bin")
    for old, suffix in [
        (bin_path,                        ".bin"        ),
        (bin_path.with_suffix(".bin.gz"), ".bin.gz"     ),
        (factory_path,                    ".factory.bin"),
        (elf_path,                        ".elf"        ),
    ]:
        if old.is_file():
            new = out_dir / f"{variant}-{ts}{suffix}"
            shutil.copy(str(old), str(new))
            print(f"  -> {new.name}")

    map_gz = tasmotapiolib.get_final_map_path(env).with_suffix(".map.gz")
    if map_gz.is_file():
        new = map_gz.parent / f"{variant}-{ts}.map.gz"
        shutil.copy(str(map_gz), str(new))
        print(f"  -> {new.name}")


_act = env.Action(_do_copy)
_act.strfunction = lambda target, source, env: ""
env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", _act)
