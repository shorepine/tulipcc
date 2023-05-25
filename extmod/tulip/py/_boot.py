import gc
import uos
import tulip, alles, world
from upysh import *
from tulip import edit, run


# This _boot runs both desktop and esp32s3, so we check for flashdev
try:
    from flashbdev import bdev
    try:
        if bdev:
            uos.mount(bdev, "/")
    except OSError as e:
        print(str(e))
        import inisetup
        vfs = inisetup.setup()
except ImportError:
    # Tulip Desktop
    tulipcc = uos.getenv("HOME")+"/Documents/tulipcc"
    try:
        mkdir(tulipcc)
    except OSError:
        # already exists, that's fine
        pass
    try:
        # If they don't have an ex & ex/g folders, make them and fill them in
        mkdir(tulipcc+"/ex")
        mkdir(tulipcc+"/ex/g")
        print("Loading new examples into ex/ folder ... ")
        if(tulip.app_path().endswith("unix")): # running in dev mode
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

gc.collect()
