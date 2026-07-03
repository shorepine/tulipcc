"""Make the in-repo amyboardctl importable when it isn't pip-installed.

The recorder's board link and World API client live in tools/amyboardctl (the
shared AMYboard control library — see docs/amyboard/control_api.md). Import
this module before `import amyboardctl` in any recorder script.
"""

import os
import sys

try:
    import amyboardctl  # noqa: F401  (pip-installed)
except ImportError:
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                    "..", "amyboardctl"))
