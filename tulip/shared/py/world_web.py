# world_web.py
# async API calling version of tulip world
# at first for just the web ports

import json
import js
from world import nice_time, headers, MAX_USERNAME_SIZE, MAX_DESCRIPTION_SIZE, _isdir, username
import tulip
import os
from upysh import pwd
import tulip_world_upload_file

# convert python dict to JS options
def options(d):
    return js.JSON.parse(json.dumps(d))

def as_bytearray(buffer):
    """
    Given a JavaScript ArrayBuffer, convert it to a Python bytearray in a
    MicroPython friendly manner.
    """
    ui8a = js.Uint8Array.new(buffer)
    size = ui8a.length
    ba = bytearray(size)
    for i in range(0, size):
        ba[i] = ui8a[i]
    return ba

URL_RFC_3986 = {
    "!": "%21", "#": "%23", "$": "%24", "&": "%26", "'": "%27", "(": "%28", ")": "%29", "*": "%2A", "+": "%2B", 
    ",": "%2C", "/": "%2F", ":": "%3A", ";": "%3B", "=": "%3D", "?": "%3F", "@": "%40", "[": "%5B", "]": "%5D",
}
def url_encoder(b):
    if type(b)==bytes:
        b = b.decode(encoding="utf-8") #byte can't insert many utf8 charaters
    result = bytearray() #bytearray: rw, bytes: read-only
    for i in b:
        if i in URL_RFC_3986:
            for j in URL_RFC_3986[i]:
                result.append(ord(j))
            continue
        i = bytes(i, "utf-8")
        if len(i)==1:
            result.append(ord(i))
        else:
            for c in i:
                c = hex(c)[2:].upper()
                result.append(ord("%"))
                result.append(ord(c[0:1]))
                result.append(ord(c[1:2]))
    result = result.decode("ascii")
    return result

def modal_url(method, **kwargs):
    joined = ""
    for x in kwargs.keys():
        joined += "%s=%s&" % (x, url_encoder(str(kwargs[x])))
    return "https://bwhitman--tulipworldapi-%s.modal.run?%s" % (method, joined[:-1])

# generic grab of modal API proxy, returns a promise of a json decode from modal
def grab(method, **kwargs):
    return js.fetch(modal_url(method, **kwargs)).then(lambda r: r.text()).then(lambda x: json.loads(x))

# generic grab of modal API proxy, returns a promise of a bytes decode from modal
def grab_bytes(method, **kwargs):
    return js.fetch(modal_url(method, **kwargs)).then(lambda r: r.arrayBuffer()).then(lambda x: as_bytearray(x))

def grab_bytes_direct(url, **kwargs):
    return js.fetch(url, options(kwargs)).then(lambda r: r.arrayBuffer()).then(lambda x: as_bytearray(x))

def post_bytes(method, filename, **kwargs):
    contents = bytes(open(filename,'rb').read())
    ui8a = js.Uint8Array.new(contents)

    file = js.File.new(ui8a, filename, options({'type':'application/binary'}))
    return js.fetch(modal_url(method), options({'method':'POST', 'body':{'file':file}}))

# get unique files
def unique_files(count=10, overquery=10):
    def do_next(all_files):
        already = {}
        ret = []
        for f in all_files:
            fn = f['username']+':'+f["filename"]
            if(not fn in already):
                already[fn] = True
                ret.append(f)
        return ret[:count]
    return grab("messages", n=count*overquery, mtype='files').then(lambda x: do_next(x))

def ls(count=10): # prints latest count files
    def do_next(x):
        print()
        for f in x:
            if(f['filename'].endswith(".tar")):
                f['filename'] = "<%s>" % (f['filename'][:-4])
            print("% 20s % 10s % 8s  %s" % (f['filename'], f['username'][:MAX_USERNAME_SIZE],nice_time(f["age_ms"]), f['content'][:MAX_DESCRIPTION_SIZE]))
    unique_files(count=count).then(lambda x: do_next(x))


def download(filename, username=None, limit=5000, chunk_size=4096, done_cb = None):
    def do_next(files):
        got = None
        def save_finished(x):
            r = open(filename, 'wb')
            r.write(bytes(x))
            r.close()
            print("Downloaded %s by %s [%d bytes, last updated %s] from Tulip World." % (filename,got['username'], got['size'], age_nice.lstrip()))
            if(filename.endswith('.tar')):
                print("Unpacking %s. Run it with run('%s')" % (filename, filename[:-4]))
                tulip.tar_extract(filename, show_progress=False)
                os.remove(filename)
            if(done_cb is not None): done_cb()

        for file in files:
            if(file["filename"] == filename):
                if username is None or username==file['username']:
                    got = file
                    break
        if got is not None:
            age_nice = nice_time(got["age_ms"])
            grab_url = got["url"] # Will get the latest (most recent) file with that name
            return grab_bytes("urlget", url=got['url']).then(lambda x: save_finished(x))
        else:
            if(username is None):
                print("Could not find %s on Tulip World" % (filename))
            else:
                print("Could not find %s by %s on Tulip World" % (filename, username))

    # Check for an extension
    if('.' not in filename[-5:]):
        filename = filename + ".tar"
    return grab("messages", n=limit, mtype="files").then(lambda x: do_next(x))

    
        
        

def post_message(message):
    if(username is None):
        print("need to type in: world.username = 'username' first.")
        return
    return grab("postmessage", content=username + " ### " + message)


# uploads a file
def upload(filename, description=""):
    if(username is None):
        print("need to type in: world.username = 'username' first.")
        return

    tar = False
    if(_isdir(filename)):
        tar = True
        print("Packing %s" % (filename))
        tulip.tar_create(filename)
        filename += ".tar"

    def clean_up(x):
        print("Uploaded %s to Tulip World." % (filename))
        if(tar):
            os.remove(filename)

    tulip_world_upload_file(pwd()+'/', filename, username, description).then(lambda x: clean_up(x))


