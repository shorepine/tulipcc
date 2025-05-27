import gc
import uos
import tulip, sys, midi, synth, amy, world,  sequencer
from upysh import *
from tulip import board

if board()!="AMYBOARD" and board()!="AMYBOARD_WEB":
    from tulip import edit, run
else:
    if(board()=="AMYBOARD"):
        import amyboard
        amyboard.mount_sd()
        amyboard.start_amy()

if board()=="WEB" or board()=="AMYBOARD_WEB":
    def webnyi():
        return "This function is not available on Tulip Web"
    import world_web as world
    input = webnyi()
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

# Make sure user/lib exists and add it to sys.path
libdir = tulip.root_dir()+"user/lib"
if(not tulip.exists(libdir)):
    mkdir(libdir)
sys.path.append(libdir)

# Remove other ones
try:
    sys.path.remove('/lib')
except ValueError:
    pass
try:
    sys.path.remove('~/.micropython/lib')
except ValueError:
    pass

gc.collect()

# Set up audio/midi. 
if(tulip.board() == "WEB" or tulip.board()=="AMYBOARD_WEB"):
    midi.setup()
    amy.AMY_SAMPLE_RATE=48000
    # Override send & bleep are done from JS on web because of click-to-start audio.
else:
    amy.AMY_SAMPLE_RATE=44100
    # Override amy's send to work with tulip
    amy.override_send = lambda x: tulip.amy_send(x)
    midi.setup() # Just mirrors the setup mostly managed by AMY

if(board() == "AMYBOARD" or board()=="AMYBOARD_WEB"):
    import amyboard
    amyboard.init_display()
    
