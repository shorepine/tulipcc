freeze_as_mpy("$(MPY_DIR)/tools", "upip.py")
freeze_as_mpy("$(MPY_DIR)/tools", "upip_utarfile.py", opt=3)
freeze("$(MPY_DIR)/extmod/tulip/py")
freeze("$(MPY_LIB_DIR)/micropython/upysh", "upysh.py")
