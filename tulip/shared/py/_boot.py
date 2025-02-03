import gc
import uos
import tulip, sys, midi, amy, alles
from upysh import *
from tulip import edit, run, board
if board()=="WEB":
    import world_web as world
else:
    import world

# This _boot runs both desktop and esp32s3


try:
    from esp32 import Partition
    try:
        s = Partition.find(Partition.TYPE_DATA, label="system")[0]
        u = Partition.find(Partition.TYPE_DATA, label="vfs")[0]
        uos.mount(s,"/sys")
        uos.mount(u,"/user")
        cd('/user')
        sys.path.append("/sys/ex")
        
    except:
        print("Tulip flash not setup properly. Try resetting the device.")

    try:
        currentPartition = Partition(Partition.RUNNING)
        currentPartition.mark_app_valid_cancel_rollback()
    except OSError:
        # this is you booting a non-OTA esp32 device. it's ok!
        pass


except ImportError:
    # Not a hardware Tulip
    try:
        tulipcc = tulip.root_dir()
        if(not tulip.exists(tulipcc)):
            mkdir(tulipcc)
    
        # On Desktop, we can put sys in sys/ and user in user/
        if(tulip.board()=='DESKTOP'):
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
        tulip.add_to_bootpy("# boot.py\n# Put anything here you want to run on Tulip startup\n", only_first_create=True)
        sys.path.append("../sys/ex")

    except Exception as e:
        sys.print_exception(e)
        # Probably iOS
        cd(tulip.app_path())

gc.collect()

# Set up audio/midi. 
if(tulip.board() == "WEB"):
    midi.setup()
    # Override send & bleep are done from JS on web because of click-to-start audio.
else:
    # Override amy's send to work with tulip
    amy.override_send = lambda x: tulip.alles_send(x, alles.mesh_flag)
    midi.setup()
    midi.startup_bleep()

