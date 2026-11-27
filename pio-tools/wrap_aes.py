# Use the ESP8266 ROM's AES-128 key expansion and encryption table.
# Define USE_FULL_AES_KEY_SETUP in build_flags if a custom build needs the SDK
# implementation, for example for WPA2-Enterprise or AES-192/256 callers.

import os
import re

Import("env")

if (env.get("BOARD_MCU") or "").lower() == "esp8266":
    build_flags = " ".join(env.get("BUILD_FLAGS") or [])
    if not re.search(r"(?:^|\s)-D\s*USE_FULL_AES_KEY_SETUP(?:=\S+)?(?=\s|$)", build_flags):
        env.Append(LINKFLAGS=[
            "-Wl,--wrap=rijndaelKeySetupEnc",
            "-Wl,--wrap=aes_encrypt",
        ])
        env.BuildSources(
            os.path.join("$BUILD_DIR", "aes_rom"),
            os.path.join(env.subst("$PROJECT_DIR"), "pio-tools"),
            src_filter=["-<*>", "+<aes_rom.cpp>", "+<aes_encrypt.cpp>"],
        )
