import runpy
import unittest
from pathlib import Path


SCRIPT = Path(__file__).resolve().parents[1] / "pio-tools" / "wrap_aes.py"


class BuildEnvironment(dict):
    def __init__(self, mcu, flags):
        super().__init__(BOARD_MCU=mcu, BUILD_FLAGS=flags)
        self.link_flags = []
        self.sources = []

    def Append(self, LINKFLAGS):
        self.link_flags.extend(LINKFLAGS)

    def BuildSources(self, target, source_dir, src_filter):
        self.sources.append((target, source_dir, src_filter))

    def subst(self, value):
        return str(SCRIPT.parent.parent)


class WrapAesTest(unittest.TestCase):
    def run_script(self, mcu, flags):
        env = BuildEnvironment(mcu, flags)
        runpy.run_path(str(SCRIPT), init_globals={"env": env, "Import": lambda name: None})
        return env

    def test_esp8266_wraps_without_printf_dependency(self):
        env = self.run_script("esp8266", ["-DUSE_FULL_PRINTF"])
        self.assertEqual(env.link_flags, ["-Wl,--wrap=rijndaelKeySetupEnc", "-Wl,--wrap=aes_encrypt"])
        self.assertEqual(env.sources[0][2], ["-<*>", "+<aes_rom.cpp>", "+<aes_encrypt.cpp>"])

    def test_opt_out(self):
        for flag in ("-DUSE_FULL_AES_KEY_SETUP", "-DUSE_FULL_AES_KEY_SETUP=1", "-D USE_FULL_AES_KEY_SETUP"):
            with self.subTest(flag=flag):
                env = self.run_script("esp8266", [flag])
                self.assertEqual(env.link_flags, [])
                self.assertEqual(env.sources, [])

    def test_similar_define_does_not_opt_out(self):
        env = self.run_script("esp8266", ["-DUSE_FULL_AES_KEY_SETUP_EXTRA"])
        self.assertTrue(env.link_flags)

    def test_other_mcu_is_untouched(self):
        env = self.run_script("esp32", [])
        self.assertEqual(env.link_flags, [])
        self.assertEqual(env.sources, [])


if __name__ == "__main__":
    unittest.main()
