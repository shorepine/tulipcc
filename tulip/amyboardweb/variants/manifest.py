# The asyncio package is built from the standard implementation but with the
# core scheduler replaced with a custom scheduler that uses the JavaScript
# runtime (with setTimeout an Promise's) to contrtol the scheduling.

freeze("../../shared/py")
freeze("../../../amy", "amy.py")
freeze("../../../amy", "juno.py")
freeze("../../../amy", "amy_wave.py")
freeze("../../../amy", "amy_constants.py")

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
