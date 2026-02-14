# amyboard.py
import tulip, midi, amy, time, struct, time

i2c = None
display_buffer = None # the bytes object storing the display bits
display = None # The framebuf object to draw to 
hw_display = None # if there's an I2C display connected

def web():
    return (tulip.board()=="AMYBOARD_WEB")

DEFAULT_ENV_SOURCE = "# Empty environment\nprint(\"Welcome to AMYboard!\")\n"
DEFAULT_PATCHES_SOURCE = "# patches.txt, ,AMY messages to load on boot\n"

def _path_exists(path):
    import os
    try:
        os.stat(path)
        return True
    except OSError:
        return False

def _remove_tree(path):
    import os
    try:
        mode = os.stat(path)[0]
    except OSError:
        return
    if mode & 0x4000:  # directory
        try:
            entries = os.ilistdir(path)
        except OSError:
            entries = []
        for entry in entries:
            name = entry[0]
            if name == "." or name == "..":
                continue
            _remove_tree(path.rstrip("/") + "/" + name)
        try:
            os.rmdir(path)
        except OSError:
            pass
    else:
        try:
            os.remove(path)
        except OSError:
            pass

def _clear_directory(path):
    import os
    try:
        entries = os.ilistdir(path)
    except OSError:
        return
    for entry in entries:
        name = entry[0]
        if name == "." or name == "..":
            continue
        _remove_tree(path.rstrip("/") + "/" + name)

def environment_transfer_done(*_args):
    import os
    import machine

    print("Environment transfer done hook called")
    user_base = tulip.root_dir() + "user"
    current_base = user_base + "/current"
    env_dir = current_base + "/env"
    for path in (user_base, current_base, env_dir):
        try:
            os.mkdir(path)
        except OSError:
            pass

    env_tar = env_dir + "/environment.tar"
    incoming_tar = current_base + "/__incoming_environment.tar"
    cwd_tar = os.getcwd().rstrip("/") + "/environment.tar"
    source_candidates = ["environment.tar", cwd_tar, current_base + "/environment.tar", env_tar]
    source_tar = None
    for candidate in source_candidates:
        if _path_exists(candidate):
            source_tar = candidate
            break
    if source_tar is None:
        print("environment transfer done hook: environment.tar not found")
        return

    if source_tar == env_tar:
        try:
            os.rename(env_tar, incoming_tar)
            source_tar = incoming_tar
        except OSError:
            pass

    _clear_directory(env_dir)

    try:
        os.rename(source_tar, env_tar)
    except OSError:
        print("environment transfer done hook: could not move environment.tar into /user/current/env")
        return

    os.chdir(env_dir)
    tulip.tar_extract("environment.tar")
    try:
        os.remove("environment.tar")
    except OSError:
        pass
    machine.reset()

def _ensure_current_env_layout():
    import os
    import uos
    user_base = tulip.root_dir() + "user"
    current_base = user_base + "/current"
    default_base = user_base + "/default"
    current_env_dir = current_base + "/env"
    default_env_dir = default_base + "/env"
    for path in (current_base, current_env_dir, default_base, default_env_dir):
        try:
            os.mkdir(path)
        except OSError:
            pass

    default_env_file = default_env_dir + "/env.py"
    default_patches_file = default_env_dir + "/patches.txt"
    current_env_file = current_env_dir + "/env.py"
    current_patches_file = current_env_dir + "/patches.txt"

    try:
        open(default_env_file, "r").close()
    except OSError:
        w = open(default_env_file, "w")
        w.write(DEFAULT_ENV_SOURCE)
        w.close()
    try:
        open(default_patches_file, "r").close()
    except OSError:
        w = open(default_patches_file, "w")
        w.write(DEFAULT_PATCHES_SOURCE)
        w.close()

    try:
        open(current_env_file, "r").close()
    except OSError:
        w = open(current_env_file, "w")
        w.write(open(default_env_file, "r").read())
        w.close()

    # Keep compatibility with old /current/patch/ location if it exists.
    legacy_patches = current_base + "/patches/patches.txt"
    try:
        uos.stat(legacy_patches)
        try:
            open(current_patches_file, "r").close()
        except OSError:
            w = open(current_patches_file, "w")
            w.write(open(legacy_patches, "r").read())
            w.close()
    except OSError:
        pass

    # Keep compatibility with older /current/patch/ location if it exists.
    legacy_patch = current_base + "/patch/patches.txt"
    try:
        uos.stat(legacy_patch)
        try:
            open(current_patches_file, "r").close()
        except OSError:
            w = open(current_patches_file, "w")
            w.write(open(legacy_patch, "r").read())
            w.close()
    except OSError:
        pass

    try:
        open(current_patches_file, "r").close()
    except OSError:
        w = open(current_patches_file, "w")
        w.write(open(default_patches_file, "r").read())
        w.close()

    return (current_env_dir, current_patches_file)

def mount_sd():
    # mount the SD card if given
    import machine, uos
    try:
        sd = machine.SDCard(sck=12, miso=13, mosi=11, cs=10,slot=2)
        uos.mount(uos.VfsFat(sd), '/sd')
        print("SD card mounted as /sd")
    except OSError:
        pass # it's ok!

def init_midi(type='A'):
    from machine import Pin
    # set up MIDI, default to type A
    # only have to set up MIDI OUT. MIDI IN will work either way
    if(type=='A'):
        # Type A: GPIO15 held high, MIDI OUT pin is 14
        pin = Pin(15, Pin.OUT)
        pin.value(1)
        return 14
    else:
        # Type B: GPIO14 held high, MIDI OUT pin is 15
        pin = Pin(14, Pin.OUT)  
        pin.value(1)
        return 15


def start_amy():
    init_pcm9211()
    midi_out_pin = init_midi()
    tulip.amyboard_start(midi_out_pin)
    if 0:
        (_env_dir, patches_file) = _ensure_current_env_layout()

        try:
            for line in open(patches_file, "r"):
                message = line.strip()
                if message and (not message.startswith("#")):
                    amy.send_raw(message)
        except OSError:
            print("No patches.txt in env")

        from upysh import cd
        cd(_env_dir)
        try:
            execfile("env.py")
        except Exception as e:
            print("Environment start failed:")
            try:
                import sys
                sys.print_exception(e)
            except Exception:
                print("Couldn't load env.py from %s" % (_env_dir))
        cd(tulip.root_dir() + "user")

def get_i2c():
    global i2c
    if(i2c is None):
        from machine import I2C
        i2c = I2C(0, freq=400000)
    return i2c

def ssd1327_oled():
    import ssd1327
    d = ssd1327.SSD1327_I2C(128,128,get_i2c(),addr=0x3d)
    d.fill(0)
    return d

def sh1107_oled():
    import sh1107
    d = sh1107.SH1107_I2C(128, 128, get_i2c(), address=0x3c)
    d.sleep(False)
    return d

def display_refresh():
    global hw_display, display_buffer
    if hw_display is not None: hw_display.show()
    if(web()): tulip.framebuf_web_update(display_buffer)

def display_startup():
    global display
    display.text("AMYboard!!!!", 0,0,255)
    display.text(tulip.version(),0,24,255)
    display_refresh()

def init_display():
    global display_buffer, display, hw_display
    if(web()):
        import framebuf
        display_buffer = bytearray(128 * 128 // 2)
        display = framebuf.FrameBuffer(display_buffer, 128, 128, framebuf.GS4_HMSB)
    else:
        try:
            hw_display = ssd1327_oled()
            display_buffer = hw_display.buffer
            display = hw_display.framebuf
        except: # not there, try the other one
            try:
                hw_display = sh1107_oled()
                display_buffer = hw_display.displaybuf
                display = hw_display 
            except:
                pass # no physical display
    if(display is not None): display_startup()

def adc1115_raw(channel=0):
    import adc1115
    adc = adc1115.ADS1115(get_i2c())
    raw = float(adc.read(channel1=channel))
    return raw

# Calibrate the ADC1115 against the GP8413. Generates a csv file
def cv_cal(channel=0):
    vs = [-10,-9,-8,-7,-6,-5,-4,-3,-2,-1.5,-1,-0.5,0,0.5,1,1.5,2,3,4,5,6,7,8,9,10]
    print("Calibrating...")
    out = open("cal.csv", "w")
    for v in vs:
        cv_out(v, channel=channel)
        time.sleep(1)
        raw = adc1115(channel)
        out.write("%f,%d\n" % (v, raw))
    out.close()
    print("Done. Written to cal.csv")

def read_register(addr, reg):
    get_i2c().writeto(addr, bytes([reg]))
    b = get_i2c().readfrom(addr, 1)
    if(b):
        return int.from_bytes(b, "big")
    return None

def write_register(addr, reg, val):
    get_i2c().writeto(addr, bytes([reg, val]))

def init_pcm9211(addr=0x40):
    registers = [
        [ 0x40, 0x33 ], # Power down ADC, power down DIR, power down DIT, power down OSC
        [ 0x40, 0xc0 ], # Normal operation for all
        [ 0x34, 0x00 ], # Initialize DIR - both biphase amps on, input from RXIN0
        [ 0x26, 0x01 ], # Main Out is DIR/ADC if no DIR sync (these match power-on default, repeated for clarity).
        [ 0x6B, 0x00 ], # Main output pins are DIR/ADC AUTO
        [ 0x30, 0x04 ], # PLL sends 512fs as SCK
        [ 0x31, 0x0A ], # XTI SCK as 512fs too
        [ 0x60, 0x44 ], # Initialize PCM9211 DIT to send SPDIF from AUXIN1 through MPO0 (pin15).  MPO1 (pin16) is VOUT (Valid)
        [ 0x78, 0x3d ], # MPO0 = 0b1101 = TXOUT, MPO1 = 0b0011 = VOUT
        [ 0x6F, 0x40 ]  # MPIO_A = CLKST etc / MPIO_B = AUXIN2 / MPIO_C = AUXIN1
    ]
    for (reg, val) in registers:
        write_register(addr, reg, val)
        r = read_register(addr, reg)
        if(r==val): 
            print("Write 0x%02x to 0x%02x OK" % (val, reg))
        else:
            print("Write 0x%02x to 0x%02x returned 0x%02x" % (val, reg, r))

def cv_out(volts, channel=0):
    """Output -10.0v to +10.0v (nominal) on CV1 (channel=0) or 2 (channel=1)"""
    addr = 88 # GP8413
    # With rev1 scaling, 0x0000 -> -10v, 0x7fff -> +10v
    val = int(((volts + 10)/20.0) * 0x8000)
    if(val < 0):
        val = 0
    if(val > 0x7fff):
        val = 0x7fff
    b1 = (val & 0xff00) >> 8
    b0 = (val & 0x00ff)
    ch = 0x02
    if(channel == 1):
        ch = 0x04
    get_i2c().writeto_mem(addr, ch, bytes([b0,b1]))

def cv_in(channel=0):
    raw = adc1115_raw(channel)
    (minr,maxr) = (1058.0, 21312.0) # measured -10v to 10v from the gp8413 output 
    return (((raw - minr) / (maxr-minr))*20.0)-10.0

# Adafruit I2C Quad Rotary Encoder Breakout
# https://www.adafruit.com/product/5752
# Four rotary encoders with built-in push buttons.
#  read_encoder(encoder=0)  # reads the current value of the encoder
#  init_buttons()  # must be called to configure pullup of encoder buttons
#  read_buttons()  # returns a boolean list of the state of the 4 buttons.
# Code based on abstracting
# https://github.com/adafruit/Adafruit_CircuitPython_seesaw/blob/main/adafruit_seesaw/seesaw.py.

def read_encoder(encoder=0, seesaw_dev=0x49, delay=0.008):
    """Read the cumulated value of encoder 0..3."""
    i2c = get_i2c()
    result = bytearray(4)
    ENCODER_BASE = 0x11
    ENCODER_POSITION = 0x30
    i2c.writeto(seesaw_dev, bytes([ENCODER_BASE, ENCODER_POSITION + encoder]))
    time.sleep(delay)
    i2c.readfrom_into(seesaw_dev, result)
    return struct.unpack(">i", result)[0]

def init_buttons(pins=(12, 14, 17, 9), seesaw_dev=0x49):
    """Setup the seesaw quad encoder button pins to input_pullup."""
    mask = 0
    for p in pins:
        mask |= (1 << p)
    mask_bytes = struct.pack('>I', mask)
    i2c = get_i2c()
    GPIO_BASE = 0x01
    GPIO_DIRCLR_BULK = 0x03
    GPIO_PULLENSET = 0x0B
    GPIO_BULK_SET = 0x05
    i2c.writeto(seesaw_dev, bytes([GPIO_BASE, GPIO_DIRCLR_BULK]) + mask_bytes)
    i2c.writeto(seesaw_dev, bytes([GPIO_BASE, GPIO_PULLENSET]) + mask_bytes)
    i2c.writeto(seesaw_dev, bytes([GPIO_BASE, GPIO_BULK_SET]) + mask_bytes)

def read_buttons(pins=(12, 14, 17, 9), seesaw_dev=0x49, delay=0.008):
    """Read the 4 seesaw encoder push buttons."""
    i2c = get_i2c()
    GPIO_BASE = 0x01
    GPIO_BULK = 0x04
    i2c.writeto(seesaw_dev, bytes([GPIO_BASE, GPIO_BULK]))
    time.sleep(delay)
    buffer = bytearray(4)
    i2c.readfrom_into(seesaw_dev, buffer)
    mask = struct.unpack('>I', buffer)[0]
    result = []
    for p in pins:
        state = True
        if (mask & (1 << p)):
            # bit set means button not pressed.
            state = False
        result.append(state)
    return result

def monitor_encoders():
    """Show status of encoders on display."""
    # You can run this in a loop to get continuous display of encoders
    # e.g.
    # >>> amyboard.init_buttons()
    # >>> for _ in range(100):
    # ...     amyboard.monitor_encoders()
    # ...
    bar_top = 20
    bar_height = 8
    bar_space = 12
    bar_center = 64
    buttons = read_buttons()
    for encoder in range(4):
        width = read_encoder(encoder)
        top = bar_top + encoder * bar_space
        # Clear existing bar
        display.fill_rect(0, top, 128, bar_space, 0)
        # Draw new bar
        if width < 0:
            center = bar_center + width
            width = -width
        else:
            center = bar_center
        display.fill_rect(center, top, width, bar_height, 1)
        if buttons[encoder]:
            # Small box at the left.
            display.fill_rect(2, top + 2, 2, 2, 1)
    display.show()

WAVEFORM_MAX = 32767.0

def draw_waveform():
    """Plot some of the output waveform on the oled."""
    # e.g.
    # >>> for _ in range(100):
    # ...     amyboard.draw_waveform()
    waveform = struct.unpack('<' + (512 * 'h'), tulip.amy_get_output_buffer())
    waveform_top = 0
    waveform_left = 0
    waveform_height = 64
    waveform_width = 128
    display.fill_rect(waveform_left, waveform_top,
                      waveform_width, waveform_height, 0)
    # center the largest sample
    max_value = 0
    max_value_pos = 0
    for x in range(len(waveform) // 2 - waveform_width):
        pos = waveform_width // 2 + x
        value = (waveform[2 * pos] + waveform[2 * pos + 1])
        if value > max_value:
            max_value = value
            max_value_pos = pos
    # Pos marks middle; shift it to the left edge
    pos = max_value_pos - waveform_width // 2
    for x in range(waveform_width):
        value = (waveform[2 * (pos + x)] + waveform[2 * (pos + x) + 1]) // 2
        y = waveform_top + int((waveform_height // 2)
                               * (1.0 - value / WAVEFORM_MAX))
        if x > 0:
            display.line(waveform_left + last_x, last_y,
                         waveform_left + x, y, 1)
        last_x = x
        last_y = y
    display.show()
