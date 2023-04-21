Import("env")

import tasmotapiolib
def firm_metrics(source, target, env):
    if env["PIOPLATFORM"] == "espressif32":
        import tasmota_metrics
        env.Execute("$PYTHONEXE -m tasmota_metrics " + str(tasmotapiolib.get_source_map_path(env).resolve()))

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin",firm_metrics)