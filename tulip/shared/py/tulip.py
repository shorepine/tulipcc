# tulip.py

# Bring in all c-defined tulip functions
from _tulip import *
from upysh import cd, pwd
import amy

if(board()!='AMYBOARD' and board()!="AMYBOARD_WEB"):
    from tulip_graphics import *
    from ui import *
    from editor import edit

def sys():
    return root_dir()+"sys/"

import midi

def startup_bleep():
    amy.send(synth=16, note=57, vel=1, time=tulip.amy_ticks_ms()+0)
    amy.send(synth=16, note=57, vel=0, time=tulip.amy_ticks_ms()+140)
    amy.send(synth=16, note=69, vel=1, time=tulip.amy_ticks_ms()+150)
    amy.send(synth=16, note=69, vel=0, time=tulip.amy_ticks_ms()+300)

# prompt for y/n and return true if Y
def prompt(prompt):
    if(board()=="WEB"):
        return webnyi()
    print(prompt + " [Yy/Nn]: ", end='')
    r = input()
    if(r=='Y' or r=='y'): 
        return True
    return

# Add a string to the users' boot.py -- used by some utlities
# if only_first_create is set, will only write if the file doesn't exist yet
def add_to_bootpy(s, only_first_create=False):
    first = False
    try:
        bootpy = open(root_dir()+"user/boot.py","r").read()
    except OSError:
        first = True
        bootpy = "" # file doesn't exist yet
    
    bootpy = bootpy + "\n" + s + "\n"
    if((only_first_create and first) or (not only_first_create)):
        w = open(root_dir()+'user/boot.py','w')
        w.write(bootpy)
        w.close()

# Wrapper around AMY tempo to store it
amy_bpm = 108
def seq_bpm(bpm=None):
    global amy_bpm
    if bpm is None:
        return amy_bpm
    else:
        amy.send(tempo=bpm)
        amy_bpm = bpm

def seq_ppq(ppq=None):
    if(ppq is not None):
        print("You can no longer set PPQ in Tulip. It's fixed at %d" % (amy.AMY_SEQUENCER_PPQ))
    return amy.AMY_SEQUENCER_PPQ    

def remap():
    if(board()=="WEB"):
        return webnyi()
    print("Type key or key combo you wish to remap: ",end='')
    (_, scan, mod) = key_wait()
    print()
    print("Type ASCII (0-127) code for the symbol you want (see https://ascii-codes.com/): ", end='')
    code = int(input())
    cmd = "tulip.key_remap(%d,%d,%d)" % (scan, mod, code)
    key_remap(scan, mod, code)
    if(prompt("Remapped. Add \"%s\" to your boot.py?" % (cmd))):
        add_to_bootpy(cmd)
        print("All done. Reboot Tulip!")

def version():
    # Returns current tulip version (aka git hash and compiled date)
    me = build_strings()
    return me[2].replace("-", "") + "-" + me[1].replace("-dirty", "")

def free_disk_bytes():
    import os
    st = os.statvfs('.')
    return st[1] * st[3]

def root_dir():
    try:
        import uos

        root_directory = "/"
        if board() == "DESKTOP":
            xdg_data_directory = uos.getenv("HOME") + "/.local/share"
            
            if (exists(xdg_data_directory)):
                root_directory = xdg_data_directory + "/tulipcc/"
            else:
                # fall back to ~/Documents if XDG base directory spec is not supported
                root_directory = uos.getenv("HOME") + "/Documents/tulipcc/"
        elif board() == "WEB":
            root_directory = "/tulip4/"
        elif board() == "AMYBOARD_WEB":
            root_directory = "/amyboard/"
        return root_directory
    except:
        return "/"

# This is only (fow now) done from macOS .app packages.
# For linux/windows, it's copied on build -- no predefined packages for those (yet)
# For iOS we work in the app folder, so no need to copy
def desktop_copy_sys(dest):
    import os
    if (app_path().endswith(".app")): # running from an app bundle
        print("Copying system folder from app bundle on first run... ")
        tulip_home = app_path()+"/Contents/Resources/tulip_home"
        cmd = "cp -Rf \"%s/\" \"%s\"" % (tulip_home, dest)
        os.system(cmd)


def get_latest_release():
    import json
    from upysh import rm
    url_save('https://api.github.com/repos/shorepine/tulipcc/releases/latest','releases_temp.json')
    j = json.load(open('releases_temp.json','r'))
    rm('releases_temp.json')
    mine = None
    sys = None
    for i in j['assets']:
        if(board()=="AMYBOARD"):
            if i['name'] == 'amyboard-firmware-AMYBOARD.bin':
                mine = i
            if i['name'] == 'amyboard-sys.bin':
                sys = i
        else:
            if i['name'] == 'tulip-firmware-%s.bin' % (board()):
                mine = i
            if i['name'] == 'tulip-sys.bin':
                sys = i
    if(mine and sys):
        return (mine['browser_download_url'], mine['updated_at'], sys['browser_download_url'], sys['updated_at'])
    return (None, None, None, None)

def upgrade():
    import time, sys, os
    import tuliprequests as urequests
    try:
        import esp32, machine
        from esp32 import Partition
    except ImportError:
        print("Upgrading only works on AMYboard or Tulip CC. Visit tulip.computer to download the latest Tulip Desktop or Tulip Web.")
        return

    if ip() is None:
        print("Need to be on wifi.")
        return

    # Ensures we are on a OTA partition
    ota_partition = Partition(esp32.Partition.RUNNING).get_next_update()
    sys_partition = Partition.find(Partition.TYPE_DATA, label="system")[0]

    if(ota_partition is None or (not ota_partition.info()[4].startswith('ota'))):
        print("You are not using OTA partitions.")
        return
    if((board() == "OTHER") or (board() == "DESKTOP")):
        print("Unidentified board type, can't upgrade.")
        return

    # Checks for a new firmware from Github, asks if you want to upgrade to it
    sec_size = 4096

    (latest_mine_url, latest_mine_date, latest_sys_url, latest_sys_date) = get_latest_release()
    if(latest_mine_url is None):
        print("Could not find firmware and system folder for board %s" % (board()))
        return

    print("For board: %s" % (board()))
    print("Latest firmware: %s" % (latest_mine_date))
    print("Latest /sys    : %s" % (latest_sys_date))
    print()
    do_firmware = False
    do_system = False

    print("Upgrade firmware?       (Y/[N]) ",end='')
    a = input()
    if(a=='Y' or a=='y'):
        do_firmware=True

    print("Upgrade /sys folder?    (Y/[N]) ",end='')
    a = input()
    if(a=='Y' or a=='y'):
        do_system=True
    
    if(do_system):
        try:
            write_size = sys_partition.info()[3]
            print("Flashing /sys folder... ")
            r = urequests.get(latest_sys_url)
            block_c = 0
            for chunk in r.generate(chunk_size=sec_size):
                if len(chunk) < sec_size:
                    print("Last chunk, adding padding of sys folder")
                    chunk = chunk + bytes(b'\xff'*(sec_size - len(chunk)))
                sys_partition.writeblocks(block_c, chunk)
                if(block_c and (block_c % 128 == 0)):
                    print("%d/%d bytes flashed of sys folder" % (block_c*sec_size, write_size))
                block_c = block_c + 1
            r.close()
            print("Write done.")
        except Exception as e:
            sys.print_exception(e)

    if(do_firmware):
        try:
            write_size = ota_partition.info()[3]
            print("Flashing to OTA partition %s " % (ota_partition.info()[4]))
            r = urequests.get(latest_mine_url)
            block_c = 0
            for chunk in r.generate(chunk_size=sec_size):
                if len(chunk) < sec_size:
                    chunk = chunk + bytes(b'\xff'*(sec_size - len(chunk)))
                ota_partition.writeblocks(block_c, chunk)
                if(block_c and (block_c % 128 == 0)):
                    print("%d/%d bytes flashed of firmware" % (block_c*sec_size, write_size))
                block_c = block_c + 1
            print("Write done. Setting boot partition to %s..." % (ota_partition.info()[4]))
            ota_partition.set_boot()
            # Partition.mark_app_valid_cancel_rollback is set in _boot.py. So if Tulip reboots properly, the partition will be marked OK
        except Exception as e:
            sys.print_exception(e)

    if(do_system or do_firmware): 
        print("Rebooting in 5 seconds...")   
        time.sleep(5)
        machine.reset()

# Return Tulip CC r11 or TDECK battery voltage in V
# This is read from a voltage divider on VBAT into an ADC.
# It's not incredibly accurate, but good enough for a "four-bar" battery indicator
# Things should turn off around 3.1V
def battery(n=5):
    if(board()=='TULIP4_R11'):
        pin = 3
    elif(board()=='TDECK'):
        pin = 4
    else:
        return 0

    import time
    from machine import Pin, ADC
    pot = ADC(Pin(pin))
    pot.atten(ADC.ATTN_11DB)
    x = 0
    for i in range(n):
        x = x + pot.read_uv()
        time.sleep(0.1)
    x = ((x / (float(n))) * 2.0) / 1000000.0
    return x

def reload(module):
    thing = module
    if(type(thing)!=str): # this is a module, convert to a str
        thing = module.__name__
    try:
        exec('del sys.modules["%s"]' % (thing))
    except KeyError:
        pass # it's ok
    if(not is_folder(thing)):
        exec('import %s' % (thing))
    else:
        cd()


def is_folder(fn):
    import os
    try:
        if(os.stat(fn)[0]&0x4000):
            return True
    except:
        pass
    return False

def exists(fn):
    import os
    try:
        r = os.stat(fn)
    except OSError:
        return False
    return True


def url_save(url, filename, mode="wb", headers={"User-Agent":"TulipCC/4.0"}):
    if(board()=="WEB"):
        import world_web, js
        def next(x):
            r = open(filename, mode)
            r.write(x)
            r.close()
            return "OK"
        return world_web.grab_bytes_direct(url, headers=headers).then(lambda x: next(x))
    else:
        import tuliprequests as urequests
        d = urequests.get(url, headers = headers).save(filename,mode)
        return d

def url_get(url, headers={"User-Agent":"TulipCC/4.0"}):
    if(board()=="WEB"):
        import world_web
        return world_web.grab_bytes_direct(url, headers=headers)
    else:
        import tuliprequests as urequests
        c = urequests.get(url, headers = headers)
        return c


def url_put(url, filename, headers={"User-Agent":"TulipCC/4.0"}):
    if(board()=="WEB"):
        return webnyi()
    import os
    import tuliprequests as urequests
    filesize = os.stat(filename)[6]
    f = open(filename, 'rb')
    put_response = urequests.put(
        url,
        headers={
            "User-Agent": headers['User-Agent'],
            "Tulip-Filename": filename,
            "Content-Type": "application/octet-stream",
        },
        data=f,
    )
    f.close()


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


def tar_create(directory):
    import utarfile
    t = utarfile.TarFile(directory+".tar", 'w')
    t.add(directory)
    t.close()

def tar_extract(file_name, show_progress=True):
    import os
    import utarfile
    tar = utarfile.TarFile(file_name, 'r')
    if(show_progress): print("extracting", file_name)
    for i in tar:
        if i.type == utarfile.DIRTYPE:
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
