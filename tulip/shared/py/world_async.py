# world-async.py
# async version of tulip world
# at first for just the web ports

import json
import js
import asyncio
import os
from world import MAX_DESCRIPTION_SIZE, MAX_USERNAME_SIZE, username, _isdir, nice_time, read_in_chunks
from world import ta, tb, text_channel_id, files_channel_id, text_base_url, files_base_url, headers, discord_epoch
from world import unique_files, ls, prompt_username


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


def post_message(message):
    u = prompt_username()
    if u is None:
        return
    r = requests.post(text_base_url+"messages", headers = headers, data =  json.dumps ( {"content":u + " ### " + message} ))







