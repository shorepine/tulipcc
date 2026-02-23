# T U L I P ~ W O R L D
# a Tulip-only chat room and file transfer service

import json
import os
import tulip

if tulip.board() != "WEB" and tulip.board() != "AMYBOARD_WEB":
    import tuliprequests as requests

MAX_DESCRIPTION_SIZE = 400
MAX_MESSAGE_SIZE = 800
MAX_USERNAME_SIZE = 20
username = None

WORLD_API_BASE = "https://tulipcc-production.up.railway.app"


def _api_url(path, **params):
    if not params:
        return WORLD_API_BASE + path
    parts = []
    for k in params.keys():
        v = str(params[k])
        parts.append("%s=%s" % (k, v.replace(" ", "%20")))
    return WORLD_API_BASE + path + "?" + "&".join(parts)


def _isdir(filename):
    return (os.stat(filename)[0] & 0o40000) > 0


# convert age to something readable
def nice_time(age_ms):
    age_s = age_ms / 1000
    if age_s < 60:
        c = "%ds" % (age_s)
    elif age_s < 60 * 60:
        c = "%dm" % (age_s / 60)
    elif age_s < 60 * 60 * 24:
        c = "%dh" % (age_s / 60 / 60)
    elif age_s < 60 * 60 * 24 * 365:
        c = "%dd" % (age_s / 60 / 60 / 24)
    else:
        c = "~"
    c = c + " ago"
    return "% 8s" % c


def _get_json(path, **params):
    r = requests.get(_api_url(path, **params))
    if r.status_code < 200 or r.status_code >= 300:
        raise Exception("HTTP %d" % (r.status_code))
    return r.json()


def _post_json(path, payload):
    r = requests.post(WORLD_API_BASE + path, json=payload)
    if r.status_code < 200 or r.status_code >= 300:
        raise Exception("HTTP %d" % (r.status_code))
    return r.json()


def _post_multipart(path, data, file_field, file_path, upload_name=None):
    boundary = "----tulipworldboundary"
    file_size = os.stat(file_path)[6]
    upload_filename = upload_name if upload_name else file_path
    media_type = "application/x-tar" if upload_filename.lower().endswith(".tar") else "application/octet-stream"

    def parts():
        for k in data.keys():
            yield ("--%s\r\n" % boundary).encode()
            yield ("Content-Disposition: form-data; name=\"%s\"\r\n\r\n" % k).encode()
            yield (str(data[k]) + "\r\n").encode()

        yield ("--%s\r\n" % boundary).encode()
        yield (
            "Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n" % (file_field, upload_filename)
        ).encode()
        yield ("Content-Type: %s\r\n\r\n" % media_type).encode()

        f = open(file_path, "rb")
        while True:
            chunk = f.read(4096)
            if not chunk:
                break
            yield chunk
        f.close()

        yield b"\r\n"
        yield ("--%s--\r\n" % boundary).encode()

    # tuliprequests sends generators as chunked transfer encoding.
    # Keep header keys/values as str to avoid bytes/str comparison warnings.
    headers = {
        "Content-Type": "multipart/form-data; boundary=%s" % (boundary),
    }

    r = requests.post(WORLD_API_BASE + path, headers=headers, data=parts())
    if r.status_code < 200 or r.status_code >= 300:
        raise Exception("HTTP %d" % (r.status_code))
    return r.json()


# get the last n messages
def messages(n=500, chunk_size=100, mtype="text"):
    if mtype == "text":
        return _get_json("/api/tulipworld/messages", limit=n)
    return _get_json("/api/tulipworld/files", limit=n, latest_per_user_env="false")["items"]


def download(filename, username=None, limit=5000, chunk_size=4096, done_cb=None):
    if "." not in filename[-5:]:
        filename = filename + ".tar"
    try:
        if username is None:
            got = _get_json("/api/tulipworld/files_latest", filename=filename)
        else:
            got = _get_json("/api/tulipworld/files_latest", filename=filename, username=username)
    except Exception:
        got = None

    if got is None:
        if username is None:
            print("Could not find %s on Tulip World" % (filename))
        else:
            print("Could not find %s by %s on Tulip World" % (filename, username))
        if done_cb:
            done_cb()
        return

    age_nice = nice_time(got["age_ms"])
    r = requests.get(got["download_url"] if got["download_url"].startswith("http") else WORLD_API_BASE + got["download_url"])
    r.save(filename)

    print(
        "Downloaded %s by %s [%d bytes, last updated %s] from Tulip World."
        % (filename, got["username"], got["size"], age_nice.lstrip())
    )
    if filename.endswith(".tar"):
        print("Unpacking %s. Run it with run('%s')" % (filename, filename[:-4]))
        tulip.tar_extract(filename, show_progress=False)
        os.remove(filename)

    if done_cb:
        done_cb()


# get unique files
def unique_files(count=10, overquery=10):
    already = {}
    ret = []
    all_files = messages(n=count * overquery, mtype="files")
    for f in all_files:
        fn = f["username"] + ":" + f["filename"]
        if fn not in already:
            already[fn] = True
            ret.append(f)
            if len(ret) == count:
                return ret
    return ret


def ls(count=10):  # prints latest count files
    for f in unique_files(count=count):
        if f["filename"].endswith(".tar"):
            f["filename"] = "<%s>" % (f["filename"][:-4])
        print(
            "% 20s % 10s % 8s  %s"
            % (
                f["filename"],
                f["username"][:MAX_USERNAME_SIZE],
                nice_time(f["age_ms"]),
                f["content"][:MAX_DESCRIPTION_SIZE],
            )
        )


# uploads a file
def upload(filename, description=""):
    u = prompt_username()
    if u is None:
        return

    tar = False
    file_path = filename
    if _isdir(filename):
        tar = True
        print("Packing %s" % (filename))
        tulip.tar_create(filename)
        file_path = filename + ".tar"

    upload_name = file_path.split("/")[-1]

    try:
        _post_multipart(
            "/api/tulipworld/upload",
            {"username": u, "description": description[:MAX_DESCRIPTION_SIZE]},
            "file",
            file_path,
            upload_name=upload_name,
        )
        print("Uploaded %s to Tulip World." % (upload_name))
    except Exception as e:
        print("Upload failed for %s (%s)" % (upload_name, e))
    finally:
        if tar:
            os.remove(file_path)


def prompt_username():
    global username
    if username is None:
        print("You need to tell us your username. Between 1 and 20 characters: ", end="")
        r = input()
        if len(r) > 0 and len(r) <= MAX_USERNAME_SIZE:
            username = r
            if tulip.prompt("Do you want me to save this username to your boot.py?"):
                tulip.add_to_bootpy("world.username='%s'" % (username))
        else:
            print("Username %s needs to be at least one character and at most %d characters." % (r, MAX_USERNAME_SIZE))
    return username


def post_message(message):
    u = prompt_username()
    if u is None:
        return
    try:
        _post_json("/api/tulipworld/messages", {"username": u, "content": message[:MAX_MESSAGE_SIZE]})
    except Exception:
        print("Could not post message.")
