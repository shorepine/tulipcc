# Frozen manifest for AMYboard VCV — mirrors tulip/macos/variants/manifest.py
# (the same firmware Python runs on hardware, web, desktop and here).
include("$(MPY_DIR)/extmod/asyncio")
freeze("$(MPY_DIR)/../tulip/shared/py")
freeze("$(MPY_DIR)/../tulip/shared/amyboard-py", "amyboard.py")
package("amy", base_path="$(MPY_DIR)/../amy")
require("bundle-networking")
