include("$(MPY_DIR)/extmod/asyncio")
freeze("$(MPY_DIR)/../tulip/shared/py")
# AMYboard's python module, so Tulip Desktop can run AMYboard World sketches
# (world.amyboard.download). CV helpers and I2C accessories no-op on desktop.
freeze("$(MPY_DIR)/../tulip/shared/amyboard-py", "amyboard.py")
package("amy", base_path="$(MPY_DIR)/../amy")

require("bundle-networking")

