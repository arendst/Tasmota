import re
import os

def run_cmd_get_output(cmd):
    return os.popen(cmd).read().strip()
