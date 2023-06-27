#require("mip-cmdline")

#freeze_as_mpy("$(MPY_DIR)/lib/micropython-lib/micropython/utarfile", "utarfile.py")
#freeze_as_mpy("$(MPY_DIR)/tools", "upip_utarfile.py", opt=3)
#freeze("$(MPY_DIR)/extmod/tulip/py")

include("$(MPY_DIR)/extmod/asyncio")
# Require some micropython-lib modules.
freeze("$(MPY_DIR)/../tulip/shared/py")
#freeze("$(MPY_DIR)/lib/micropython-lib/micropython/utarfile", "utarfile.py")
