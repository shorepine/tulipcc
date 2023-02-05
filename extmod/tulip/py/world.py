# T U L I P ~ W O R L D
# a Tulip only chat room and file transfer service

import os
import ubinascii as ub
import urequests
import json
import tulip

# TODO: i should verify Tulip-ness some other way . I can revoke this token easily enough but why not do it per user?
world_token = "syt_dHVsaXA_lPADiXCwKdCJvreALSul_0ody9J"
host = "duraflame.rosaline.org"
room_id = "!rGPkdYQOECXDlTVoGe:%s" % (host)

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


# PUT call to matrix CS api using auth
def matrix_put(url, data):
    return urequests.put(url, data=bytes(json.dumps(data).encode('utf-8')), headers={"Authorization":"Bearer %s" %(world_token)})

# GET call to matrix CS api using auth
def matrix_get(url):
    return urequests.get(url, headers={"Authorization":"Bearer %s" %(world_token)})

# Send a m.room.message to the tulip room
def send(message):
    data={"msgtype":"m.text","body":message}
    url="https://%s/_matrix/client/v3/rooms/%s/send/%s/%s" % (host, room_id, "m.room.message", str(uuid4()))
    matrix_put(url, data)

# Do the initial sync of the room 
def sync(limit=100):
    m = []
    url = "https://%s/_matrix/client/r0/rooms/%s/initialSync?limit=%d" % (host,room_id,limit)
    data = matrix_get(url)
    end = data.json()['messages']['end']
    for e in data.json()['messages']['chunk']:
        if(e['type']=='m.room.message'):
            m.append({"body":e['content']['body'], "age_s":int(e['age']/1000)})
    return (m, end)

# given a end from a sync or the last check, see if there's new messages. 
def check(end,limit=100):
    m = []
    url = "https://%s/_matrix/client/r0/rooms/%s/messages?from=%s&dir=f&limit=%d" % (host, room_id, end, limit)
    data = matrix_get(url)
    end = data.json()['end']
    for e in data.json()['chunk']:
        if(e['type']=='m.room.message'):
            m.append({"body":e['content']['body'], "age_s":int(e['age']/1000)})    
    return (m,end)

# covert age from matrix to something readable
def nice_time(age_s):
    if(age_s < 60):
        c = "%ds" % (age_s)
    elif(age_s < 60*60):
        c = "%dm" % (age_s/60)
    elif(age_s < 60*60*24):
        c = "%dh" % (age_s/60/60)
    elif(age_s < 60*60*24*100):
        c = "%dd" % (age_s/60/60/24)
    else: 
        c= "~"
    c = c + " ago"
    return "% 8s" % c

def put_message(m):
    print("~~~~ " + nice_time(m["age_s"]) + ": " + m["body"])

# gets called every screen frame 
def frame_callback(data):
    data["count"] += 1
    # Check if we've waited long enough to read again 
    if(data["count"] > (data["fps"] * data["update_s"])):
        data["count"] = 0 # reset the counter even if we're still reading, will just wait another 10s
        # Check that we're not still reading 
        if(data["read"] is False):
            data["read"] = True # semaphore! ! lol 
            (m,data["end"]) = check(data["end"])
            for i in m:
                # do something with the message
                put_message(i)
            data["read"] = False

# sets up the UI for world
def world_ui():
    if(tulip.ip() is None):
        print("need wifi.")
        return
    tulip.gpu_reset()
    print(tulip.Colors.INVERSE + "Welcome to T U L I P ~ W O R L D")
    print("please enter your name: ")
    print(tulip.Colors.DEFAULT)
    name = input()
    print("OK, hey " + name + ", i'll load it up")

    # Get the initial set of n last messages
    (m,e) = sync()
    for i in m:
        print(nice_time(i["age_s"]) + " - " + i["body"])

    # And set up a timer callback (on the GPU) to read more every so often
    data = {}
    data["count"] = 0
    data["fps"] = tulip.fps()
    data["update_s"] = 10 # 10sec
    data["end"] = e
    data["read"] = False
    tulip.frame_callback(frame_callback, data)
    # And return ... immediately to the REPL? No, probably should show a UI with a text entry field, etc

# stops the frame callback, returns to repl
def quit():
    tulip.frame_callback()
    tulip.gpu_reset()



