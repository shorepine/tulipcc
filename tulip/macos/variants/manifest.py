include("$(MPY_DIR)/extmod/asyncio")
freeze("$(MPY_DIR)/../tulip/shared/py")
freeze("$(MPY_DIR)/../amy", "amy.py")
freeze("$(MPY_DIR)/../amy", "juno.py")
freeze("$(MPY_DIR)/../amy", "amy_wave.py")
freeze("$(MPY_DIR)/../amy/experiments", "tulip_piano.py")
freeze("$(MPY_DIR)/../amy/experiments", "piano_params.py")

