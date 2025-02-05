# Just not _boot, we have our own
freeze("$(PORT_DIR)/modules", "apa106.py")
freeze("$(PORT_DIR)/modules", "inisetup.py")
freeze("$(PORT_DIR)/modules", "espnow.py")
freeze("$(PORT_DIR)/modules", "flashbdev.py")

include("$(MPY_DIR)/extmod/asyncio")

# Useful networking-related packages.
#require("mip")
require("ntptime")
#require("webrepl")

# Require some micropython-lib modules.
# require("aioespnow")
require("dht")
require("ds18x20")
require("onewire")
require("umqtt.robust")
require("umqtt.simple")

freeze("$(PORT_DIR)/../shared/py")
freeze("$(MPY_DIR)/../amy", "amy.py")
freeze("$(MPY_DIR)/../amy", "juno.py")
freeze("$(MPY_DIR)/../amy", "amy_wave.py")
freeze("$(MPY_DIR)/../amy", "amy_constants.py")
#freeze("$(MPY_DIR)/lib/micropython-lib/micropython/utarfile", "utarfile.py")
