Import("env")

import tasmotapiolib

if env["PIOPLATFORM"] == "espressif32":
    def firm_metrics(source, target, env):
        # Print Metrics for firmware using "map" file from ".pio/build/..."
        import tasmota_metrics
        env.Execute("$PYTHONEXE -m tasmota_metrics " + str(tasmotapiolib.get_source_map_path(env).resolve()))

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin",firm_metrics)