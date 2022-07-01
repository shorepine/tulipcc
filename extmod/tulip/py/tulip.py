# tulip.py


# Bring in all c-defined tulip functions
from _tulip import * 

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

def url_save(url, filename, mode="wb", headers={"User-Agent":"TulipCC/4.0"}):
    import urequests
    return urequests.get(url, headers = headers).save(filename,mode)

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
    import network
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



