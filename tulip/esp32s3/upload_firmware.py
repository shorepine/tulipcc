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

world_token = "syt_dHVsaXA_lPADiXCwKdCJvreALSul_0ody9J"
host = "duraflame.rosaline.org"
firmware_room_id = "!eMmMZLncsdKrMOFTMM:%s" % (host)

# micropython version of uuid from micropython-lib
class UUID:
    def __init__(self, bytes):
        if len(bytes) != 16:
            raise ValueError('bytes arg must be 16 bytes long')
        self._bytes = bytes

    @property
    def hex(self):
        return ub.hexlify(self._bytes).decode()

    def __str__(self):
        h = self.hex
        return '-'.join((h[0:8], h[8:12], h[12:16], h[16:20], h[20:32]))

    def __repr__(self):
        return "<UUID: %s>" % str(self)

def uuid4():
    """Generates a random UUID compliant to RFC 4122 pg.14"""
    random = bytearray(os.urandom(16))
    random[6] = (random[6] & 0x0F) | 0x40
    random[8] = (random[8] & 0x3F) | 0x80
    return UUID(bytes=random)

def matrix_post(url, data, content_type="application/octet-stream"):
    return requests.post(url, data=data, headers={"Authorization":"Bearer %s" %(world_token), "Content-Type":content_type})

# PUT call to matrix CS api using auth
def matrix_put(url, data):
    return requests.put(url, data=bytes(json.dumps(data).encode('utf-8')), headers={"Authorization":"Bearer %s" %(world_token)})

# Uploads a file or folder from Tulip to Tulip World
def upload(filename, content_type="application/octet-stream"):
    # Don't upload this here anymore after the next release, we use github releases
    if 1:
        url = "https://%s/_matrix/media/v3/upload?filename=%s" % (host, filename)
        file = open(filename, "rb").read()
        uri = matrix_post(url, file, content_type=content_type).json()["content_uri"]
        data={"info":{"mimetype":content_type},"msgtype":"m.file","body":filename,"url":uri}
        url="https://%s/_matrix/client/v3/rooms/%s/send/%s/%s" % (host, firmware_room_id, "m.room.message", str(uuid4()))
        matrix_put(url, data)
        print("Uploaded %s to Tulip World." % (filename))



info = makeversionhdr.get_version_info_from_git('.')
build_date = datetime.date.today()
bd_p = build_date.strftime("%Y%m%d")
board = subprocess.check_output("grep MICROPY_BOARD build/CMakeCache.txt | cut -d '=' -f2 | awk '{print $1}'",shell=True)[:-1].decode('ascii')

# Send an arg to this script to upload sys.
if(len(sys.argv) == 1):
    firmware_fn = "tulipcc-" + board + "-" + bd_p + "-" + info[1].replace("-dirty", "") + ".bin"
    print(firmware_fn)
    os.system("cp build/micropython.bin %s" % (firmware_fn))
    upload(firmware_fn)
    os.remove(firmware_fn)
else:
    sys_fn = "sys-" + bd_p + "-" + info[1].replace("-dirty", "") + ".bin"
    print(sys_fn)
    os.system("cp build/tulip-sys.bin %s" % (sys_fn))
    upload(sys_fn)
    os.remove(sys_fn)
