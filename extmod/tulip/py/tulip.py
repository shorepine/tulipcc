# tulip.py


# Bring in all c-defined tulip functions
from _tulip import *
from world import world
from upysh import cd

class Colors:
    """ ANSI color codes """
    BLACK = "\033[0;30m"
    RED = "\033[0;31m"
    GREEN = "\033[0;32m"
    BROWN = "\033[0;33m"
    BLUE = "\033[0;34m"
    PURPLE = "\033[0;35m"
    CYAN = "\033[0;36m"
    LIGHT_GRAY = "\033[0;37m"
    DARK_GRAY = "\033[1;30m"
    LIGHT_RED = "\033[1;31m"
    LIGHT_GREEN = "\033[1;32m"
    YELLOW = "\033[1;33m"
    LIGHT_BLUE = "\033[1;34m"
    LIGHT_PURPLE = "\033[1;35m"
    LIGHT_CYAN = "\033[1;36m"
    LIGHT_WHITE = "\033[1;37m"
    BG_BLACK = "\033[0;40m"
    BG_RED = "\033[0;41m"
    BG_GREEN = "\033[0;42m"
    BG_BROWN = "\033[0;43m"
    BG_BLUE = "\033[0;44m"
    BG_PURPLE = "\033[0;45m"
    BG_CYAN = "\033[0;46m"
    BG_LIGHT_GRAY = "\033[0;47m"
    BG_DARK_GRAY = "\033[1;40m"
    BG_LIGHT_RED = "\033[1;41m"
    BG_LIGHT_GREEN = "\033[1;42m"
    BG_YELLOW = "\033[1;43m"
    BG_LIGHT_BLUE = "\033[1;44m"
    BG_LIGHT_PURPLE = "\033[1;45m"
    BG_LIGHT_CYAN = "\033[1;46m"
    BG_LIGHT_WHITE = "\033[1;47m"
    BOLD = "\033[1m"
    FAINT = "\033[2m"
    ITALIC = "\033[3m"
    UNDERLINE = "\033[4m"
    FLASH = BLINK = "\033[5m"
    REVERSE = INVERSE = NEGATIVE = "\033[7m"
    STRIKE = CROSSED = "\033[9m"
    RESET = DEFAULT = END = "\033[0m"
    SORTED_HUE = [0,73,146,219,251,178,105,210,242,137,169,201,233,32,64,96,128,160,192,224,228,196,237,164,205,132,232,246,173,100,200,241,168,236,214,141,209,68,136,204,240,245,172,250,177,104,208,244,213,140,249,176,212,248,255,182,109,218,254,145,181,217,253,36,72,108,144,180,216,252,220,184,221,148,185,112,188,222,149,76,152,189,116,156,186,113,153,40,80,120,124,157,84,190,117,44,88,92,121,48,125,52,56,60,223,150,77,154,158,81,85,89,93,4,8,12,16,20,24,28,61,57,126,53,29,122,49,25,94,21,191,118,45,90,62,17,30,58,159,86,13,26,127,54,95,22,63,31,187,114,155,41,82,123,91,50,59,9,18,27,23,55,87,14,119,46,19,51,151,78,83,5,10,15,47,115,42,11,79,6,43,7,183,110,147,37,74,111,75,38,39,1,2,3,35,71,107,34,143,70,67,103,179,106,139,33,66,99,135,175,102,131,171,98,167,163,215,142,211,69,138,207,203,134,199,65,130,195,227,231,235,162,239,166,243,170,97,194,198,226,247,174,101,202,230,129,234,161,206,133,193,225,238,165,197,229]

class Joy:
    # These are the mask bits for the joystick / keyboard
    # TODO: test NES (not just SNES)
    # Use like (tulip.joy & tulip.Joy.SELECT)
    R1 = 2
    L1 = 4
    X = 8
    A = 16
    RIGHT = 32
    LEFT = 64
    DOWN = 128
    UP = 256
    START = 512
    SELECT = 1024
    Y = 2048
    B = 4096

# like joy, but also scans the keyboard. lets you use either
# Z = B, X = A, A = Y, S = X, enter = START, ' = SELECT, Q = L1, W = R1, arrows = DPAD
def joyk():
    jmask = joy()
    key_scans = keys()[1:5] # get up to four keys held at once
    for k in key_scans:
        if(k == 79): jmask = jmask | Joy.RIGHT
        if(k == 80): jmask = jmask | Joy.LEFT
        if(k == 82): jmask = jmask | Joy.UP
        if(k == 81): jmask = jmask | Joy.DOWN
        if(k == 29): jmask = jmask | Joy.B
        if(k == 27): jmask = jmask | Joy.A
        if(k == 4):  jmask = jmask | Joy.Y
        if(k == 22): jmask = jmask | Joy.X
        if(k == 40): jmask = jmask | Joy.START
        if(k == 52): jmask = jmask | Joy.SELECT
        if(k == 20): jmask = jmask | Joy.L1
        if(k == 26): jmask = jmask | Joy.R1
    return jmask

# runs and cleans up a Tulip "app", which is a folder named X with a file called X.py inside
# TODO - pass args
def run(module):
    import gc, sys
    cd(module)
    try:
        exec('import %s' %  (module))
    except KeyboardInterrupt:
        pass
    exec('del sys.modules["%s"]' % (module))
    gc.collect()
    gc.disable()
    cd('..')


def url_save(url, filename, mode="wb", headers={"User-Agent":"TulipCC/4.0"}):
    import urequests
    display_stop()
    d = urequests.get(url, headers = headers).save(filename,mode)
    display_start()
    return d

def url_get(url, headers={"User-Agent":"TulipCC/4.0"}):
    import urequests
    c = urequests.get(url, headers = headers)
    return c

def screenshot(filename=None):
    from upysh import rm
    if(filename is not None):
        int_screenshot(filename)
        return None
    if(ip() is not None):
        int_screenshot("_temp_ss.png")
        import urequests
        url = "https://api.imgur.com/3/image"
        auth = "Client-ID 3939c2d3f5a6f83"
        ct = "multipart/form-data;"
        data = open("_temp_ss.png","rb").read()
        r = urequests.post(url, data=data, headers={"authorization":auth, "content-type":ct}).content
        rm('_temp_ss.png')
        return "https://i.imgur.com/%s.png" % (r.decode('ascii')[15:22])
    return "Need wi-fi on"


def ansi_fg(pal_idx):
    # ESC[38;5;{ID}m
    return("\033[38;5;%dm" % (pal_idx))

def ansi_bg(pal_idx):
    # ESC[48;5;{ID}m
    return("\033[48;5;%dm" % (pal_idx))

def color(r,g,b):
    ret = 0;
    ret |= (r&0xe0);
    ret |= (g&0xe0) >> 3
    ret |= (b&0xc0) >> 6
    return (ret & 0xff)

# TODO, the wide version too??
def rgb(px0):
    r = px0 & 0xe0;
    g = (px0 << 3) & 0xe0
    b = (px0 << 6) & 0xc0
    return (r,g,b)

def ip():
    try:
        import network
    except ImportError:
        return "127.0.0.1" # we are on local and it's ok
    sta_if = network.WLAN(network.STA_IF)
    if(sta_if.isconnected()):
        return sta_if.ifconfig()[0]
    else:
        return None

def set_time():
    import ntptime
    if(ip() is None):
        print("Need to be on wifi")
        return
    ntptime.settime()


def wifi(ssid, passwd, wait_timeout=10):
    import network, time
    sta_if = network.WLAN(network.STA_IF)
    sta_if.active(True)
    sta_if.connect(ssid, passwd)
    sleep = 0
    while((sta_if.isconnected() is False) and sleep < wait_timeout):
        sleep = sleep + 1
        time.sleep(1)
    return ip()

def screen_size():
    s_s = timing()
    return (s_s[0], s_s[1])

def tar_create(directory):
    from upip import tarfile
    t = tarfile.TarFile(directory+".tar", 'w')
    t.add(directory)
    t.close()

def tar_extract(file_name, show_progress=True):
    import os
    from upip import tarfile
    display_stop()

    tar = tarfile.TarFile(file_name, 'r')
    if(show_progress): print("extracting", file_name)
    for i in tar:
        if i.type == tarfile.DIRTYPE:
            if i.name != './':
                try:
                    os.mkdir(i.name.strip('/'))
                except OSError as error:
                    if(show_progress): print("Warning: directory",i.name,"already exists")
            if(show_progress): print("making:", i.name)
        else:
            if i.name.startswith(".") or "._" in i.name:
                if(show_progress): print("ignoring", i.name)
            else:
                if(show_progress): print("extracting:", i.name)
                sub_file = tar.extractfile(i)
                data = sub_file.read()
                try:
                    with open(i.name, "wb") as dest:
                        dest.write(data)
                        dest.close()
                except OSError as error:
                    if(show_progress): print("borked on:", i.name)
    display_start()
