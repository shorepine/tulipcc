import gc
import uos
import tulip, world

import alles
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
        
    except:
        print("Tulip flash not setup properly. Run tulip_fs_create.py.")

    try:
        currentPartition = Partition(Partition.RUNNING)
        currentPartition.mark_app_valid_cancel_rollback()
    except OSError:
        # this is you booting a non-OTA esp32 device. it's ok!
        pass
    
except ImportError:
    # Tulip Desktop
    try:
        tulipcc = uos.getenv("HOME")+"/Documents/tulipcc"
        try:
            mkdir(tulipcc)
        except OSError:
            # already exists, that's fine
            pass
        try:
            # TODO: clean this up , this is not relevant anymore
            
            # If they don't have an ex & ex/g folders, make them and fill them in
            mkdir(tulipcc+"/ex")
            mkdir(tulipcc+"/ex/g")
            print("Loading new examples into ex/ folder ... ")
            if(tulip.app_path().endswith("macos")): # running in dev mode
                tulip_home = tulip.app_path()+"/../../tulip_home"
            else: # running from an app bundle
                tulip_home = tulip.app_path()+"/Contents/Resources/tulip_home"
            for file in uos.listdir(tulip_home+"/ex"):
                if(file.endswith(".py")):
                    cp(tulip_home+"/ex/"+file, tulipcc+"/ex/"+file)
            for file in uos.listdir(tulip_home+"/ex/g"):
                if(file.endswith(".png")):
                    cp(tulip_home+"/ex/g/"+file, tulipcc+"/ex/g/"+file)
        except OSError:
            # already exists, that's fine
            pass
        cd(uos.getenv("HOME")+"/Documents/tulipcc")
    except:
        # Probably iOS
        cd(tulip.app_path())

gc.collect()
