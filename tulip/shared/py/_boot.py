import gc
import uos
import tulip, world, sys, midi, amy, alles

from upysh import *
from tulip import edit, run
# This _boot runs both desktop and esp32s3
try:
    from esp32 import Partition
    try:
        s = Partition.find(Partition.TYPE_DATA, label="system")[0]
        u = Partition.find(Partition.TYPE_DATA, label="vfs")[0]
        uos.mount(s,"/sys")
        uos.mount(u,"/user")
        cd('/user')
        sys.path.append("/sys/app")
        
    except:
        print("Tulip flash not setup properly. Try resetting the device.")

    try:
        currentPartition = Partition(Partition.RUNNING)
        currentPartition.mark_app_valid_cancel_rollback()
    except OSError:
        # this is you booting a non-OTA esp32 device. it's ok!
        pass
    
except ImportError:
    # Tulip Desktop
    try:
        tulipcc = tulip.root_dir()
        try:
            mkdir(tulipcc)
        except OSError:
            # already exists, that's fine
            pass

        # On Desktop, we can put sys in sys/ and user in user/
        try:
            mkdir(tulipcc+"sys")
            tulip.desktop_copy_sys(tulipcc+"sys")
        except OSError:
            # already exists, do nothing
            pass

        try:
            mkdir(tulipcc+"user")
        except OSError:
            # Already exists, don't do anything
            pass

        cd(tulip.root_dir()+"user")
        sys.path.append("../sys/app")

    except:
        # Probably iOS
        cd(tulip.app_path())

gc.collect()
# Override amy's send to work with tulip
amy.override_send = lambda x: tulip.alles_send(x, alles.mesh_flag)
alles.local() # start in local mode 
midi.setup()

