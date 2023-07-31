from os.path import join
import subprocess
from SCons.Script import COMMAND_LINE_TARGETS

Import("env")
env = DefaultEnvironment()
platform = env.PioPlatform()

if "upload" in COMMAND_LINE_TARGETS:

    def esp32_detect_flashsize():
        esptoolpy = join(platform.get_package_dir("tool-esptoolpy") or "", "esptool.py")
        esptoolpy_flags = ["flash_id"]
        esptoolpy_cmd = [env["PYTHONEXE"], esptoolpy] + esptoolpy_flags
        try:
            output = subprocess.run(esptoolpy_cmd, capture_output=True).stdout.splitlines()
            for l in output:
                if l.decode().startswith("Detected flash size: "):
                    size = (l.decode().split(": ")[1])
                    print("Did get flash size:",size)
                    return size, True
            return "4MB",False
        except subprocess.CalledProcessError as exc:
            print("Did get chip info failed with " + str(exc))
            return "4MB",False

    size, overridden = esp32_detect_flashsize()
    old_flash_size = env.BoardConfig().get("upload.flash_size")
    old_maximum_size = env.BoardConfig().get("upload.maximum_size")
    new_maximum_size = int(size.split("MB")[0]) * 0x100000

    if new_maximum_size > old_maximum_size:

        extra_flags = env.BoardConfig().get("build.extra_flags").split(" ")
        new_flags = ""
        for flag in extra_flags:
            if flag.startswith("-DESP32"):
                flag = f"-DESP32_{size}"
            new_flags += flag + " "
        env.BoardConfig().update("build.extra_flags",new_flags)
        env.BoardConfig().update("upload.flash_size",size)

        env.BoardConfig().update("upload.maximum_size", new_maximum_size)

        env.Replace(TASMOTA_flash_size=size)