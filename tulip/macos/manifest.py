require("mip-cmdline")
freeze_as_mpy("$(MPY_DIR)/lib/micropython-lib/micropython/utarfile", "utarfile.py")
#freeze_as_mpy("$(MPY_DIR)/tools", "upip_utarfile.py", opt=3)
freeze("$(MPY_DIR)/extmod/tulip/py")
