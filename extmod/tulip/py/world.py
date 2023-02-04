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
            m.append({"body":e['content']['body'], "age_s":int(e['age']/1000)})
    return (m, end)

# given a end from a sync or the last check, see if there's new messages. 
def check(end):
    m = []
    url = "https://%s/_matrix/client/r0/rooms/%s/messages?from=%s&dir=f&limit=128" % (host, room_id, end)
    data = matrix_get(url)
    end = data.json()['end']
    for e in data.json()['chunk']:
        if(e['type']=='m.room.message'):
            m.append({"body":e['content']['body'], "age_s":int(e['age']/1000)})    
    return (m,end)

def register_callback(cb):
    callback = cb

def message_received(message):
    if callback is not None:
        callback(message)

def nice_time(age_s):
    if(age_s < 60):
        return "%d seconds ago" % (age_s)
    if(age_s < 60*60):
        return "%d minutes ago" % (age_s / 60)
    if(age_s < 60*60*24):
        return "%d hours ago" % (age_s / 60 / 60)
    return "%d days ago" % (age_s / 60 / 60 / 24)

def world_ui():
    import tulip, time
    if(tulip.ip() is None):
        print("need wifi.")
        return
    tulip.gpu_reset()
    print(tulip.Colors.INVERSE + "Welcome to T U L I P ~ W O R L D")
    print("please enter your name: ")
    print(tulip.Colors.DEFAULT)
    name = input()
    print("OK, hey " + name + ", i'll load it up")
    (m,e) = sync()
    for i in m:
        print(nice_time(i["age_s"]) + " - " + i["body"])
    while(True):
        time.sleep(5)
        (m, e) = check(e)
        for i in m:
            print(nice_time(i["age_s"]) + " - " + i["body"])



