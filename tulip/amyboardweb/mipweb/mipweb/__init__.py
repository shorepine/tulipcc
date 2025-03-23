# MicroPython package installer
# MIT license; Copyright (c) 2022 Jim Mussared

# Modified for webassembly `js.fetch` by Brian Whitman Feb 2025

from micropython import const
import js, json
import sys


_PACKAGE_INDEX = const("https://micropython.org/pi/v2")
_CHUNK_SIZE = 128

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

def grab_bytes(url, **kwargs):
    return js.fetch(url, options(kwargs)).then(lambda r: r.arrayBuffer()).then(lambda x: as_bytearray(x))

def grab_json(url, **kwargs):
    return js.fetch(url, options(kwargs)).then(lambda r: r.text()).then(lambda x: json.loads(x))

# This implements os.makedirs(os.dirname(path))
def _ensure_path_exists(path):
    import os

    split = path.split("/")

    # Handle paths starting with "/".
    if not split[0]:
        split.pop(0)
        split[0] = "/" + split[0]

    prefix = ""
    for i in range(len(split) - 1):
        prefix += split[i]
        try:
            os.stat(prefix)
        except:
            os.mkdir(prefix)
        prefix += "/"


# Copy from src (stream) to dest (function-taking-bytes)
def _chunk(src, dest):
    buf = memoryview(bytearray(_CHUNK_SIZE))
    while True:
        n = src.readinto(buf)
        if n == 0:
            break
        dest(buf if n == _CHUNK_SIZE else buf[:n])


# Check if the specified path exists and matches the hash.
def _check_exists(path, short_hash):
    import os

    try:
        import binascii
        import hashlib

        with open(path, "rb") as f:
            hs256 = hashlib.sha256()
            _chunk(f, hs256.update)
            existing_hash = str(binascii.hexlify(hs256.digest())[: len(short_hash)], "utf-8")
            return existing_hash == short_hash
    except:
        return False


def _rewrite_url(url, branch=None):
    if not branch:
        branch = "HEAD"
    if url.startswith("github:"):
        url = url[7:].split("/")
        url = (
            "https://raw.githubusercontent.com/"
            + url[0]
            + "/"
            + url[1]
            + "/"
            + branch
            + "/"
            + "/".join(url[2:])
        )
    elif url.startswith("gitlab:"):
        url = url[7:].split("/")
        url = (
            "https://gitlab.com/"
            + url[0]
            + "/"
            + url[1]
            + "/-/raw/"
            + branch
            + "/"
            + "/".join(url[2:])
        )
    return url


def _download_file(url, dest):
    def write(b):
        with open(dest, 'wb') as f:
            f.write(b)
        return True
    return grab_bytes(url).then(lambda bs: write(b))

def _install_json(package_json_url, index, target, version, mpy):
    def next(package_json):
        paths_hashs = package_json.get('hashes', ())
        paths_urls= package_json.get('urls', ())
        deps_dep_versions = package_json.get("deps", ())

        def process_path_hash(i):
            if(i==len(paths_hashs)): return
            target_path = paths_hashs[i][0]
            short_hash = paths_hashs[i][1]
            fs_target_path = target + "/" + target_path
            if _check_exists(fs_target_path, short_hash):
                print("Exists:", fs_target_path)
            else:
                file_url = "{}/file/{}/{}".format(index, short_hash[:2], short_hash)
                _download_file(file_url,fs_target_path).then(lambda x: process_path_hash(i+1))
        def process_path_url(i):
            if(i==len(paths_urls)): return
            target_path = paths_urls[i][0]
            url = paths_urls[i][1]
            fs_target_path = target + "/" + target_path
            _download_file(_rewrite_url(url, version), fs_target_path).then(lambda x: process_path_url(i+1))
        def process_dep_dep_version(i):
            if(i==len(deps_dep_versions)): return
            dep = deps_dep_versions[i][0]
            dep_version = deps_dep_versions[i][1]
            if not _install_package(dep, index, target, dep_version, mpy):
                return False
        process_path_hash(0).then(process_path_uri(0)).then(process_dep_dep_version(0))

    grab_json(_rewrite_url(package_json_url, version)).then(lambda x: next(x))
        return True


def _install_package(package, index, target, version, mpy):
    if (
        package.startswith("http://")
        or package.startswith("https://")
        or package.startswith("github:")
        or package.startswith("gitlab:")
    ):
        if package.endswith(".py") or package.endswith(".mpy"):
            print("Downloading {} to {}".format(package, target))
            return _download_file(
                _rewrite_url(package, version), target + "/" + package.rsplit("/")[-1]
            )
        else:
            if not package.endswith(".json"):
                if not package.endswith("/"):
                    package += "/"
                package += "package.json"
            print("Installing {} to {}".format(package, target))
    else:
        if not version:
            version = "latest"
        print("Installing {} ({}) from {} to {}".format(package, version, index, target))

        mpy_version = (
            sys.implementation._mpy & 0xFF if mpy and hasattr(sys.implementation, "_mpy") else "py"
        )

        package = "{}/package/{}/{}/{}.json".format(index, mpy_version, package, version)

    return _install_json(package, index, target, version, mpy)


def install(package, index=None, target=None, version=None, mpy=True):
    if not target:
        for p in sys.path:
            if p.endswith("/lib"):
                target = p
                break
        else:
            print("Unable to find lib dir in sys.path")
            return

    if not index:
        index = _PACKAGE_INDEX

    if _install_package(package, index.rstrip("/"), target, version, mpy):
        print("Done")
    else:
        print("Package may be partially installed")
