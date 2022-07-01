import gc
import uos
import tulip, alles
from flashbdev import bdev
from upysh import *
from tulip import edit

try:
    if bdev:
        uos.mount(bdev, "/")
except OSError:
    import inisetup

    vfs = inisetup.setup()

gc.collect()
