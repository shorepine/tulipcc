include("$(MPY_DIR)/extmod/asyncio")
freeze("$(MPY_DIR)/../tulip/shared/py")
package("amy", base_path="$(MPY_DIR)/../amy")

require("bundle-networking")

