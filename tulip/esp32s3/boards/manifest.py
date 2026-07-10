# Just not _boot, we have our own
freeze("$(PORT_DIR)/modules", "apa106.py")
freeze("$(PORT_DIR)/modules", "inisetup.py")
freeze("$(PORT_DIR)/modules", "espnow.py")
freeze("$(PORT_DIR)/modules", "flashbdev.py")
require("bundle-networking")

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
# AMYboard's python module, so Tulip can run AMYboard World sketches
# (world.amyboard.download). CV helpers no-op on Tulip; I2C accessories work.
freeze("$(PORT_DIR)/../shared/amyboard-py", "amyboard.py")
package("amy", base_path="$(MPY_DIR)/../amy")

#freeze("$(MPY_DIR)/lib/micropython-lib/micropython/utarfile", "utarfile.py")
