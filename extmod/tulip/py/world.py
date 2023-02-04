# world.py
# T U L I P ~ W O R L D
import os
import ubinascii as ub
import urequests
import json

world_token = "syt_dHVsaXA_lPADiXCwKdCJvreALSul_0ody9J"
room_id = "!rGPkdYQOECXDlTVoGe:duraflame.rosaline.org"
host = "duraflame.rosaline.org"
callback = None

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

def matrix_put(url, data):
    return urequests.put(url, data=bytes(json.dumps(data).encode('utf-8')), headers={"Authorization":"Bearer %s" %(world_token)})

def matrix_get(url):
    return urequests.get(url, headers={"Authorization":"Bearer %s" %(world_token)})

def send(message):
    data={"msgtype":"m.text","body":message}
    url="https://%s/_matrix/client/v3/rooms/%s/send/%s/%s" % (host, room_id, "m.room.message", str(uuid4()))
    matrix_put(url, data)

def sync():
    m = []
    url = "https://%s/_matrix/client/r0/rooms/%s/initialSync?limit=128" % (host,room_id)
    data = matrix_get(url)
    end = data.json()['messages']['end']
    for e in data.json()['messages']['chunk']:
        if(e['type']=='m.room.message'):
            m.append({"body":e['content']['body'], "ts":e['origin_server_ts']})
    return (m, end)

# given a end from a sync or the last check, see if there's new messages. 
def check(end):
    m = []
    # 6.3.6   GET /_matrix/client/r0/rooms/{roomId}/messages
    url = "https://%s/_matrix/client/r0/rooms/%s/messages?from=%s&dir=f&limit=128" % (host, room_id, end)
    data = matrix_get(url)
    end = data.json()['end']
    for e in data.json()['chunk']:
        if(e['type']=='m.room.message'):
            m.append({"body":e['content']['body'], "ts":e['origin_server_ts']})    
    return (m,end)

def register_callback(cb):
    callback = cb

def message_received(message):
    if callback is not None:
        callback(message)




