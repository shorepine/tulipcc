# T U L I P ~ W O R L D
# a Tulip only chat room and file transfer service

import json
import os
import tulip
if(tulip.board()!="WEB"):
    import urequests as requests

MAX_DESCRIPTION_SIZE=50
MAX_USERNAME_SIZE=10
username = None

def read_in_chunks(file_object, chunk_size=4096):
    while True:
        data = file_object.read(chunk_size)
        if not data:
            break
        yield data

def _isdir(filename):
    return (os.stat(filename)[0] & 0o40000) > 0

# covert age from discord to something readable
def nice_time(age_ms):
    age_s = age_ms / 1000
    if(age_s < 60):
        c = "%ds" % (age_s)
    elif(age_s < 60*60):
        c = "%dm" % (age_s/60)
    elif(age_s < 60*60*24):
        c = "%dh" % (age_s/60/60)
    elif(age_s < 60*60*24*365):
        c = "%dd" % (age_s/60/60/24)
    else: 
        c= "~"
    c = c + " ago"
    return "% 8s" % c



# This is the token for the Tulip World app. 
# it only has perms to be able to read and post to the private Tulip World channels on the SPSS discord
# if there's any abuse of this, i can revoke the token
ta = 'MTIzOTIyNTc4NDU3NzgxODc0NQ.GjGMum'
tb = 'KvPGzKZDr1phrId9iY7LMtIDgMNtI0om8MsWsA'
text_channel_id = "1239226672046407824" # tulip-world channel
files_channel_id = "1239512482025050204" # tulip-world-files channel

# Discord HTTP API stuff
text_base_url = "https://discordapp.com/api/channels/{}/".format(text_channel_id)
files_base_url = "https://discordapp.com/api/channels/{}/".format(files_channel_id)
headers = { "Authorization":"Bot {}".format(ta+'.'+tb),
            "User-Agent":"TulipCC/4.0 (https://tulip.computer, v0.1)",
            "Content-Type":"application/json", }

discord_epoch = 1420070400000

# get the last n messages
def messages(n=500, chunk_size = 100, mtype='text'):
    ret = []
    before = None

    base_url = files_base_url
    if(mtype == 'text'): base_url = text_base_url

    # First one is the newest
    # https://discord.com/developers/docs/resources/channel#get-channel-messages
    if(n<chunk_size): chunk_size = n
    response = requests.get(base_url+"messages?limit=%d" % (chunk_size), headers = headers)
    # We get a x-ratelimit-reset from the headers here, we can use that to know what time it is
    # We have to do it this way because (1) micropython does not have datetime/dateutil
    # and (2) we often do not know what time it is on Tulip 
    server_time_ms = (int(response.headers['x-ratelimit-reset'])-1)*1000

    # Pull up to n messages from discord, using paging
    all_responses = response.json()
    while(len(all_responses)<n and len(response.json())==chunk_size):
        oldest_id = response.json()[-1]['id']
        response = requests.get(base_url+"messages?limit=%d&before=%s" % (chunk_size, oldest_id), headers=headers)
        all_responses = all_responses + response.json()


    for i in all_responses[:n]:
        try:
            # Discord trims spaces at the end of content
            if(i['content'].endswith('#')): i['content']=i['content']+' '
            (username, content) = i['content'].split(" ### ")
        except ValueError: # not a valid TW message / file
            continue
        r = {
            # Discord IDs have epoch ms (since 2015) encoded in them
            'age_ms':server_time_ms - ((int(i['id']) >> 22) + discord_epoch), 
            'time':((int(i['id']) >> 22) + discord_epoch),
            'username':username,
            'content':content
        }
        if mtype=='text' and len(i['attachments']) == 0:
            ret.append(r)

        if mtype=='files' and len(i['attachments']) > 0:
            a = i['attachments'][0]
            r.update({
                'filename':a['filename'], 
                'size':a['size'], 
                'url':a['url'], 
                'content_type':a['content_type']
            })
            ret.append(r)
    return ret

def download(filename, username=None, limit=5000, chunk_size=4096):
    got = None
    # Check for an extension
    if('.' not in filename[-5:]):
        filename = filename + ".tar"
    for file in messages(n=limit, mtype='files'):
        if(file["filename"] == filename):
            if username is None or username==file['username']:
                got = file
                break
    if got is not None:
        age_nice = nice_time(got["age_ms"])
        grab_url = got["url"] # Will get the latest (most recent) file with that name

        r = tulip.url_save(got['url'], filename)

        print("Downloaded %s by %s [%d bytes, last updated %s] from Tulip World." % (filename,got['username'], got['size'], age_nice.lstrip()))
        if(filename.endswith('.tar')):
            print("Unpacking %s. Run it with run('%s')" % (filename, filename[:-4]))
            tulip.tar_extract(filename, show_progress=False)
            os.remove(filename)
    else:
        if(username is None):
            print("Could not find %s on Tulip World" % (filename))
        else:
            print("Could not find %s by %s on Tulip World" % (filename, username))


# get unique files
def unique_files(count=10, overquery=10):
    already = {}
    i = 0
    ret= []
    all_files = messages(n=count*overquery, mtype='files')
    for f in all_files:
        fn = f['username']+':'+f["filename"]
        if(not fn in already):
            already[fn] = True
            ret.append(f)
            if(len(ret)==count):
                return ret
    return ret

def ls(count=10): # prints latest count files
    for f in unique_files(count=count):
        if(f['filename'].endswith(".tar")):
            f['filename'] = "<%s>" % (f['filename'][:-4])
        print("% 20s % 10s % 8s  %s" % (f['filename'], f['username'][:MAX_USERNAME_SIZE],nice_time(f["age_ms"]), f['content'][:MAX_DESCRIPTION_SIZE]))

# uploads a file
def upload(filename, description=""):
    u = prompt_username()
    if u is None:
        return

    tar = False
    if(_isdir(filename)):
        tar = True
        print("Packing %s" % (filename))
        tulip.tar_create(filename)
        filename += ".tar"

    filesize = os.stat(filename)[6]
    f = open(filename, 'rb')

    # First get the url to upload to
    api_response = requests.post(
        files_base_url+"attachments",
        headers=headers,
        json={"files": [{"filename": filename, "file_size": filesize, "id": 1}]},
    )

    # Then PUT the file to the url
    attachment_info = api_response.json()["attachments"][0]
    put_url = attachment_info["upload_url"]
    put_response = requests.put(
        put_url,
        headers={
            "Authorization": headers['Authorization'],
            "User-Agent": headers['User-Agent'],
            "Content-Length": str(filesize),
            "Content-Type": "application/octet-stream",
        },
        data=f,
    )

    # Lastly, post a message with the uploaded filename
    payload = {
        "content":u + " ### " + description[:MAX_DESCRIPTION_SIZE],
        "attachments": [{
            "id": attachment_info['id'],
            "uploaded_filename":attachment_info['upload_filename'],
            "filename":filename
        }]
    }
    r = requests.post(files_base_url+"messages", headers = headers, data = json.dumps(payload))

    print("Uploaded %s to Tulip World." % (filename))
    if(tar):
        os.remove(filename)

def prompt_username():
    global username
    if(username is None):
        print("You need to tell us your username. Between 1 and 10 characters: ",end='')
        r = input()
        if(len(r)>0 and len(r)<=MAX_USERNAME_SIZE):
            username = r
            if(tulip.prompt("Do you want me to save this username to your boot.py?")):
                tulip.add_to_bootpy("world.username='%s'" % (username))
        else:
            print("Username %s needs to be at least one character and at most %d characters." % (r, MAX_USERNAME_SIZE))
    return username

def post_message(message):
    u = prompt_username()
    if u is None:
        return
    r = requests.post(text_base_url+"messages", headers = headers, data =  json.dumps ( {"content":u + " ### " + message} ))







