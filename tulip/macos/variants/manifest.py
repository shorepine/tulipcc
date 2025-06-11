
include("$(MPY_DIR)/extmod/asyncio")
freeze("$(MPY_DIR)/../tulip/shared/py")
freeze("$(MPY_DIR)/../amy", "amy.py")
freeze("$(MPY_DIR)/../amy", "juno.py")
freeze("$(MPY_DIR)/../amy", "amy_wave.py")
freeze("$(MPY_DIR)/../amy", "amy_constants.py")
freeze("$(MPY_DIR)/../amy", "example_patches.py")
require("bundle-networking")

