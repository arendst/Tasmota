from os.path import join, isfile

Import("env")

if "-DPIO_FRAMEWORK_ARDUINO_LWIP2_IPV6_HIGHER_BANDWIDTH" in env['BUILD_FLAGS']:

  FRAMEWORK_DIR = env.PioPlatform().get_package_dir("framework-arduinoespressif8266")
  patchflag_path = join(FRAMEWORK_DIR, ".patching-done")

  # patch file only if we didn't do it before
  if not isfile(join(FRAMEWORK_DIR, ".patching-done")):
    original_file = join(FRAMEWORK_DIR, "libraries", "ESP8266WiFi", "src", "ESP8266WiFiGeneric.cpp")
    patched_file = join("patches", "1-framework-arduinoespressif8266-change_dns_gethostbyname.patch")

    print("Patching " + original_file)

    assert isfile(original_file) and isfile(patched_file)

    env.Execute("patch %s %s" % (original_file, patched_file))

    def _touch(path):
        with open(path, "w") as fp:
            fp.write("")

    env.Execute(lambda *args, **kwargs: _touch(patchflag_path))
