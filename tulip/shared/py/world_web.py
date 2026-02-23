# world_web.py
# async API version of Tulip World for web ports

import json
import js
from world import nice_time, MAX_USERNAME_SIZE, MAX_DESCRIPTION_SIZE, _isdir, username
import tulip
import os
from upysh import pwd

if tulip.board() == "WEB":
    import tulip_world_upload_file as world_upload_file
if tulip.board() == "AMYBOARD_WEB":
    import amyboard_world_upload_file as world_upload_file

WORLD_API_BASE = "https://tulipcc-production.up.railway.app"


# convert python dict to JS options
def options(d):
    return js.JSON.parse(json.dumps(d))


def as_bytearray(buffer):
    # Note: we have a patch in Tulip so that Uint8Array == bytes
    ui8a = js.Uint8Array.new(buffer)
    return ui8a


def world_api_url(path_and_query=""):
    return WORLD_API_BASE + path_and_query


# Compatibility shim for existing worldui.py callers.
# Old methods:
# - messages
# - urlget
# - postmessage
def grab(method, **kwargs):
    if method == "messages":
        n = kwargs.get("n", 25)
        mtype = kwargs.get("mtype", "text")
        if mtype == "text":
            return js.fetch(world_api_url("/api/tulipworld/messages?limit=%d" % (n))).then(lambda r: r.text()).then(lambda x: json.loads(x))
        return js.fetch(
            world_api_url("/api/tulipworld/files?limit=%d&latest_per_user_env=false" % (n))
        ).then(lambda r: r.text()).then(lambda x: json.loads(x)).then(lambda payload: payload.get("items", []))

    if method == "urlget":
        url = kwargs.get("url", "")
        return js.fetch(url).then(lambda r: r.arrayBuffer()).then(lambda x: as_bytearray(x))

    if method == "postmessage":
        return js.fetch(
            world_api_url("/api/tulipworld/messages"),
            options(
                {
                    "method": "POST",
                    "headers": {"content-type": "application/json"},
                    "body": json.dumps({"username": username, "content": kwargs.get("content", "")}),
                }
            ),
        ).then(lambda r: r.text()).then(lambda x: json.loads(x))

    return js.Promise.resolve([])


def grab_bytes_direct(url, **kwargs):
    return js.fetch(url, options(kwargs)).then(lambda r: r.arrayBuffer()).then(lambda x: as_bytearray(x))


# default helper for messages
def world_print(x):
    for r in x:
        print("[%s ago] %s: %s" % (nice_time(r["age_ms"]), r["username"], r["content"]))


def messages(n=25, mtype="text", done=lambda x: world_print(x)):
    grab("messages", n=n, mtype=mtype).then(lambda x: done(x))


# get unique files
def unique_files(count=10, overquery=10):
    def do_next(all_files):
        already = {}
        ret = []
        for f in all_files:
            fn = f["username"] + ":" + f["filename"]
            if fn not in already:
                already[fn] = True
                ret.append(f)
        return ret[:count]

    return grab("messages", n=count * overquery, mtype="files").then(lambda x: do_next(x))


def ls(count=10):  # prints latest count files
    def do_next(x):
        print()
        for f in x:
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

    unique_files(count=count).then(lambda x: do_next(x))


def download(filename, username=None, limit=5000, chunk_size=4096, done_cb=None):
    # Check for an extension
    if "." not in filename[-5:]:
        filename = filename + ".tar"

    query = "/api/tulipworld/files_latest?filename=%s" % (js.encodeURIComponent(filename))
    if username is not None:
        query = query + "&username=%s" % (js.encodeURIComponent(username))

    def on_missing(_=None):
        if username is None:
            print("Could not find %s on Tulip World" % (filename))
        else:
            print("Could not find %s by %s on Tulip World" % (filename, username))
        if done_cb is not None:
            done_cb()

    def on_got(got):
        def save_finished(x):
            r = open(filename, "wb")
            r.write(bytes(x))
            r.close()
            print(
                "Downloaded %s by %s [%d bytes, last updated %s] from Tulip World."
                % (filename, got["username"], got["size"], age_nice.lstrip())
            )
            if filename.endswith(".tar"):
                print("Unpacking %s. Run it with run('%s')" % (filename, filename[:-4]))
                tulip.tar_extract(filename, show_progress=False)
                os.remove(filename)
            if done_cb is not None:
                done_cb()

        age_nice = nice_time(got["age_ms"])
        grab_url = got["download_url"]
        if not str(grab_url).startswith("http"):
            grab_url = world_api_url(grab_url)
        return grab("urlget", url=grab_url).then(lambda x: save_finished(x))

    def on_resp(resp):
        if not resp.ok:
            on_missing()
            return js.Promise.resolve(None)
        return resp.text().then(lambda x: on_got(json.loads(x)))

    return js.fetch(world_api_url(query)).then(lambda resp: on_resp(resp))


def post_message(message):
    if username is None:
        print("need to type in: world.username = 'username' first.")
        return
    return grab("postmessage", content=message)


# uploads a file
def upload(filename, description=""):
    if username is None:
        print("need to type in: world.username = 'username' first.")
        return

    tar = False
    if _isdir(filename):
        tar = True
        print("Packing %s" % (filename))
        tulip.tar_create(filename)
        filename += ".tar"

    def clean_up(x):
        print("Uploaded %s to Tulip World." % (filename))
        if tar:
            os.remove(filename)

    world_upload_file(pwd() + "/", filename, username, description).then(lambda x: clean_up(x))
