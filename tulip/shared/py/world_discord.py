# discord_world.py
# connect to Tulip World via discord

import requests
import json
import os

# This is the token for the Tulip World app. 
# it only has perms to be able to read and post to the private Tulip World channels on the SPSS discord
token = ''
text_channel_id = "1239226672046407824" # tulip-world channel
files_channel_id = "1239512482025050204" # tulip-world-files channel

# Discord HTTP API stuff
text_base_url = "https://discordapp.com/api/channels/{}/".format(text_channel_id)
files_base_url = "https://discordapp.com/api/channels/{}/".format(files_channel_id)
headers = { "Authorization":"Bot {}".format(token),
            "User-Agent":"TulipCC/4.0 (https://tulip.computer, v0.1)",
            "Content-Type":"application/json", }

discord_epoch = 1420070400000

# get the last n messages (both files and text)
def messages(n=500, chunk_size = 20, mtype='files'):
    ret = []
    before = None

    base_url = files_base_url
    if(mtype == 'text'): base_url = text_base_url

    # First one is the latest
    # Next batch is &before=id (last of the prev batch)
    # https://discord.com/developers/docs/resources/channel#get-channel-messages
    response = requests.get(base_url+"messages?limit=%d" % (chunk_size), headers = headers)
    # We get a x-ratelimit-reset from the headers here, we can use that to know what time it is
    # We have to do it this way because (1) micropython does not have datetime/dateutil
    # and (2) we often do not know what time it is on Tulip 
    server_time_ms = (int(response.headers['x-ratelimit-reset'])-1)*1000
    response = response.json()

    for i in response:
        try:
            (username, content) = i['content'].split(" ### ")
        except ValueError: # not a valid TW message / file
            continue
        r = {
            # This is the Discord username. Will be Tulip World for most of this
            #'author':i['author']['username'], 

            # Discord IDs have epoch ms (since 2015) encoded in them
            'age_ms':server_time_ms - ((int(i['id']) >> 22) + discord_epoch), 
            
            'id':i['id'],
            'username':username,
            'content':content
        }
        if mtype=='text' and len(i['attachments']) == 0:
            ret.append(r)

        if mytype=='files' and len(i['attachments']) > 0:
            a = i['attachments'][0]
            r.update({
                'filename':a['filename'], 
                'size':a['size'], 
                'url':a['url'], 
                'content_type':a['content_type']
            })
            ret.append(r)
    return ret

# uploads a file
def upload(filename, description="", username="unknown"):
    filesize = os.path.getsize(filename)
    f = open(filename, 'rb')

    # First get the url to upload to
    api_response = requests.post(
        files_base_url+"attachments",
        headers=headers,
        json={"files": [{"filename": filename, "file_size": filesize, "id": 1}]},
    )
    api_response.raise_for_status()

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
    put_response.raise_for_status()

    # Lastly, post a message with the uploaded filename
    payload = {
        "attachments": [{
            "id": attachment_info['id'],
            "uploaded_filename":attachment_info['upload_filename'],
            "filename":filename,
            "description":username + " ### " + description
        }]
    }
    r = requests.post(files_base_url+"messages", headers = headers, data = json.dumps(payload))


def post_message(message, username="unknown"):
    r = requests.post(base_url+"messages", headers = headers, data =  json.dumps ( {"content":username + " ### " + message} ))

print(str(messages(n=10)))
#post_message('hello')
#upload('test.tar', "a test tar file")
#print(str(r))


