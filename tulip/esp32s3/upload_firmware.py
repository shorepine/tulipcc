# upload_firmware.py
# uploads the most recent build to Tulip World
import sys, os
sys.path.append('../../micropython/py')
import makeversionhdr
import subprocess
import datetime
import requests
import binascii as ub
import json


info = makeversionhdr.get_version_info_from_git('.')
build_date = datetime.date.today()
bd_p = build_date.strftime("%Y%m%d")
board = subprocess.check_output("grep MICROPY_BOARD build/CMakeCache.txt | cut -d '=' -f2 | awk '{print $1}'",shell=True)[:-1].decode('ascii')

# Send an arg to this script to upload sys.
if(len(sys.argv) == 1):
    firmware_fn = "tulipcc-" + board + "-" + bd_p + "-" + info[1].replace("-dirty", "") + ".bin"
    print(firmware_fn)
    os.system("cp build/micropython.bin %s" % (firmware_fn))
    #upload(firmware_fn)
    os.remove(firmware_fn)
else:
    sys_fn = "sys-" + bd_p + "-" + info[1].replace("-dirty", "") + ".bin"
    print(sys_fn)
    os.system("cp build/tulip-sys.bin %s" % (sys_fn))
    #upload(sys_fn)
    os.remove(sys_fn)
