Import("env")

import sys
from pathlib import Path

base_dir = env['PROJECT_DIR']
unishox_dir = Path(base_dir, 'tools', 'unishox')
sys.path.append(str(unishox_dir.resolve()))
sys.dont_write_bytecode = True

compress_dir = __import__('compress-html-uncompressed').compress_dir

path_uncompressed = Path(base_dir, 'tasmota', 'html_uncompressed')
path_compressed   = Path(base_dir, 'tasmota', 'html_compressed')
compress_dir(path_uncompressed, path_compressed)
