import gc
import uos
import tulip, alles
from upysh import *
from tulip import edit

# This _boot runs both desktop and esp32s3, so we check for flashdev
try:
    from flashbdev import bdev
    try:
        if bdev:
            uos.mount(bdev, "/")
    except OSError:
        import inisetup
        vfs = inisetup.setup()
except ImportError:
    # Tulip Desktop
    try:
        mkdir(uos.getenv("HOME")+"/Documents/tulipcc")
    except OSError: #exists
        pass
    cd(uos.getenv("HOME")+"/Documents/tulipcc")

gc.collect()
