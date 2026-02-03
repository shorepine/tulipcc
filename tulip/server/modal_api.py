import modal
import json
import os
import requests
from fastapi import FastAPI, Response, Form, Body
from fastapi import File, UploadFile, HTTPException
from typing import List

MAX_DESCRIPTION_SIZE=50
MAX_USERNAME_SIZE=10
username = None

# This is the token for the Tulip World app. 
# it only has perms to be able to read and post to the private Tulip World channels on the SPSS discord
# if there's any abuse of this, i can revoke the token
ta = 'MTIzOTIyNTc4NDU3NzgxODc0NQ.GjGMum'
tb = 'KvPGzKZDr1phrId9iY7LMtIDgMNtI0om8MsWsA'
text_channel_id = "1239226672046407824" # tulip-world channel
files_channel_id = "1239512482025050204" # tulip-world-files channel
amyboard_channel_id = "1353327346924388422" # amyboard-world-files

# Discord HTTP API stuff
text_base_url = "https://discordapp.com/api/channels/{}/".format(text_channel_id)
files_base_url = "https://discordapp.com/api/channels/{}/".format(files_channel_id)
amyboard_base_url = "https://discordapp.com/api/channels/{}/".format(amyboard_channel_id)

headers = { "Authorization":"Bot {}".format(ta+'.'+tb),
            "User-Agent":"TulipCC/4.0 (https://tulip.computer, v0.1)",
            "Content-Type":"application/json", }

discord_epoch = 1420070400000

image = modal.Image.debian_slim().pip_install("fastapi[standard]", "requests", "python-multipart")
app = modal.App("tulipworldapi", image=image)

@app.function()
@modal.fastapi_endpoint()
def postmessage(content: str):
    r = requests.post(text_base_url+"messages", headers = headers, data =  json.dumps ( {"content":content} ))

@app.function()
@modal.fastapi_endpoint()
def urlget(url: str):
    r = requests.get(url)
    return Response(content=bytes(r.content), media_type="application/binary")

# get the last n messages
@app.function()
@modal.fastapi_endpoint()
def messages(n: int=500, chunk_size: int = 100, mtype: str='text'):
    ret = []
    before = None

    base_url = files_base_url
    if(mtype == 'text'): base_url = text_base_url
    if(mtype == 'amyboard'): base_url = amyboard_base_url

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

        if (mtype=='files' or mtype=='amyboard') and len(i['attachments']) > 0:
            a = i['attachments'][0]
            r.update({
                'filename':a['filename'], 
                'size':a['size'], 
                'url':a['url'], 
                'content_type':a['content_type']
            })
            ret.append(r)
    return ret

@app.function()
@modal.fastapi_endpoint()
def upload(username: str = Form(...), description: str = Form(...), which: str = Form(...), file: UploadFile = File(...)):
    contents = file.file.read()
    filename = file.filename
    filesize = len(contents)
    if(filesize==0): return {"error":"couldn't read file %s" % (filename)}

    bu = files_base_url
    if(which=='amyboard'):
        bu = amyboard_base_url

    # First get the url to upload to
    api_response = requests.post(
        bu+"attachments",
        headers=headers,
        json={"files": [{"filename": filename, "file_size": filesize, "id": 1}]},
    )
    print(api_response.json())
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
        data=contents,
    )

    # Lastly, post a message with the uploaded filename
    payload = {
        "content":username + " ### " + description[:MAX_DESCRIPTION_SIZE],
        "attachments": [{
            "id": attachment_info['id'],
            "uploaded_filename":attachment_info['upload_filename'],
            "filename":filename
        }]
    }
    r = requests.post(bu+"messages", headers = headers, data = json.dumps(payload))

    return {"ok":True} # not needed




