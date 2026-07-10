# The asyncio package is built from the standard implementation but with the
# core scheduler replaced with a custom scheduler that uses the JavaScript
# runtime (with setTimeout an Promise's) to contrtol the scheduling.

freeze("../../shared/py")
# AMYboard's python module, so Tulip Web can run AMYboard World sketches
# (world.amyboard.download). CV helpers and I2C accessories no-op on web.
freeze("../../shared/amyboard-py", "amyboard.py")
package("amy", base_path="$(MPY_DIR)/../amy")


package(
    "asyncio",
    (
        "event.py",
        "funcs.py",
        "lock.py",
    ),
    base_path="$(MPY_DIR)/extmod",
    opt=3,
)

package(
    "asyncio",
    (
        "__init__.py",
        "core.py",
    ),
    base_path="$(PORT_DIR)",
    opt=3,
)
