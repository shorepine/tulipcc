# The asyncio package is built from the standard implementation but with the
# core scheduler replaced with a custom scheduler that uses the JavaScript
# runtime (with setTimeout an Promise's) to contrtol the scheduling.
freeze("../../../../amy", "amy.py")
freeze("../../../../amy/experiments", "tulip_piano.py")
freeze("../../../../amy/experiments", "piano_params.py")
freeze("../../../../amy", "juno.py")

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
