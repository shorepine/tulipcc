import gc
import uos
import tulip, alles
from upysh import *
from tulip import edit

# This _boot runs both local and esp32s3, so we check for flashdev
try:
    from flashbdev import bdev
    try:
        if bdev:
            uos.mount(bdev, "/")
    except OSError:
        import inisetup
        vfs = inisetup.setup()
except ImportError:
    pass

gc.collect()
