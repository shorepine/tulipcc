# amyboard.py
import tulip, midi, amy, time, struct, time, sys, os

i2c = None
display = None # Display instance (unified interface for all display types)

def web():
    return (tulip.board()=="AMYBOARD_WEB")


class Display:
    """Unified display interface for ssd1327, sh1107, and web framebuffer.

    Drawing methods (text, fill_rect, line, fill, pixel, rect, scroll) are
    forwarded to the underlying framebuf.  ``show()`` does the right thing
    for each backend so callers never need to know which hardware is present.
    """
    WIDTH = 128
    HEIGHT = 128
    LINEHEIGHT = 12

    def __init__(self):
        self._hw = None        # hardware driver (ssd1327 or sh1107), None for web
        self._fb = None        # framebuf used for drawing
        self._buf = None       # raw byte buffer backing the framebuf
        self._is_web = web()

        if self._is_web:
            import framebuf
            self._buf = bytearray(self.WIDTH * self.HEIGHT // 2)
            self._fb = framebuf.FrameBuffer(self._buf, self.WIDTH, self.HEIGHT, framebuf.GS4_HMSB)
        else:
            # Try ssd1327 first, then sh1107
            try:
                hw = ssd1327_oled()
                self._hw = hw
                self._buf = hw.buffer
                self._fb = hw.framebuf
            except Exception:
                try:
                    hw = sh1107_oled()
                    self._hw = hw
                    self._buf = hw.displaybuf
                    self._fb = hw  # sh1107 *is* a FrameBuffer subclass
                except Exception:
                    pass  # no physical display

    @property
    def buffer(self):
        return self._buf

    @property
    def available(self):
        return self._fb is not None

    # -- drawing primitives (forwarded to framebuf) --

    def text(self, string, x, y, col=255):
        if self._fb is not None:
            self._fb.text(string, x, y, col)

    def fill(self, col):
        if self._fb is not None:
            self._fb.fill(col)

    def fill_rect(self, x, y, w, h, col):
        if self._fb is not None:
            self._fb.fill_rect(x, y, w, h, col)

    def line(self, x1, y1, x2, y2, col):
        if self._fb is not None:
            self._fb.line(x1, y1, x2, y2, col)

    def pixel(self, x, y, col=None):
        if self._fb is None:
            return None
        if col is None:
            return self._fb.pixel(x, y)
        self._fb.pixel(x, y, col)

    def rect(self, x, y, w, h, col):
        if self._fb is not None:
            self._fb.rect(x, y, w, h, col)

    def scroll(self, dx, dy):
        if self._fb is not None:
            self._fb.scroll(dx, dy)

    # Next-layer-up API
    def clear(self):
        self.fill(0)
        self.show()

    def message(self, message, row=0, inverse=False):
        """Display some text on a row on the display."""
        top_y = self.LINEHEIGHT * row
        bw = (255, 0) if inverse else (0, 255)
        self.fill_rect(0, top_y, self.WIDTH, self.LINEHEIGHT, bw[0])
        self.text(message, 0, top_y, bw[1])
        self.show()

    # -- refresh --

    def show(self):
        """Push the framebuffer to the display hardware (or web bridge)."""
        if self._fb is None:
            return
        if self._is_web:
            tulip.framebuf_web_update(self._buf)
        elif self._hw is not None:
            self._hw.show()

    # convenience alias so old code calling display_refresh() on a Display works
    refresh = show

DEFAULT_ENV_SOURCE = "# Put your own code here to run in your environment\n"

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


def load_patch_file(filename, synth=1, num_voices=6):
    """Load a .patch file (list of AMY wire commands) into a given synth."""

    def _num_oscs_for_patch(patch):
        """Scan for how many oscs are used in a patch."""
        top_osc = 0
        for line in patch.split('\n'):
            if not line:
                continue
            if line[0] == 'v':
                osc_num = int(line[1])  # Cannot see osc nums > 9.
            if osc_num > top_osc:
                top_osc = osc_num
        return top_osc + 1

    with open(filename, 'r') as f:
        patch = f.read()
    # Reset global FX.
    amy.send_raw("V1x0,0,0M0,500,,0,0k0,320,0.5,0.5h0,0.85,0.5,3000")
    # Reset the synth.
    oscs_per_voice = _num_oscs_for_patch(patch)
    amy.send_raw('i%div%din%d' % (synth, num_voices, oscs_per_voice))
    # Execute the patch.
    for line in patch.split('\n'):
        if line and not line.startswith('#'):
            amy.send_raw('i%d%s' % (synth, line))


def ensure_user_environment():
    import os
    user_base = tulip.root_dir() + "user"
    current_base = user_base + "/current"
    for path in (user_base, current_base):
        try:
            os.mkdir(path)
        except OSError:
            pass

    current_env_file = current_base + "/env.py"

    try:
        open(current_env_file, "r").close()
    except OSError:
        w = open(current_env_file, "w")
        w.write(DEFAULT_ENV_SOURCE)
        w.close()

    return current_base

def _ensure_current_env_layout():
    return ensure_user_environment()

def _parse_editor_state_synth_map(editor_state_path):
    try:
        raw = open(editor_state_path, "r").read()
    except OSError:
        return None
    if not raw:
        return {}
    try:
        import ujson as json
    except ImportError:
        import json
    try:
        parsed = json.loads(raw)
    except Exception:
        return {}
    if not isinstance(parsed, dict):
        return {}
    synths = parsed.get("synths", {})
    if not isinstance(synths, dict):
        return {}
    return synths

def _iter_patch_messages(path):
    try:
        for line in open(path, "r"):
            msg = line.strip()
            if msg and (not msg.startswith("#")):
                yield msg
    except OSError:
        return

def _num_oscs_from_patch_file(path):
    """Scan a .patch/.dirty file for highest v<N> line to determine oscs_per_voice."""
    max_osc = -1
    try:
        for line in open(path, "r"):
            msg = line.strip()
            if msg and msg.startswith("v"):
                # Extract osc number from e.g. "v0o0w0..." or "v7..."
                i = 1
                while i < len(msg) and msg[i].isdigit():
                    i += 1
                if i > 1:
                    osc = int(msg[1:i])
                    if osc > max_osc:
                        max_osc = osc
    except OSError:
        pass
    return 3 if max_osc < 0 else max_osc + 1

def restore_patch_state_from_files(env_dir=None, send_default_if_missing=True):
    _env_dir = env_dir or _ensure_current_env_layout()
    editor_state_file = _env_dir + "/editor_state.json"
    synth_map = _parse_editor_state_synth_map(editor_state_file)
    result = {
        "has_editor_state": synth_map is not None,
        "loaded_channels": [],
        "dirty_channels": [],
    }
    if synth_map is None:
        synth_map = {}

    # Scan for .dirty files not covered by editor_state.json
    import os
    try:
        entries = os.listdir(_env_dir)
    except OSError:
        entries = []
    for entry in entries:
        if entry.endswith(".dirty"):
            try:
                synth = int(entry[:-6])
            except Exception:
                continue
            if synth < 1 or synth > 16:
                continue
            key = str(synth)
            if key not in synth_map:
                synth_map[key] = ""

    if not synth_map:
        if send_default_if_missing:
            amy.send_raw("i1K257iv6")
        return result

    for key in synth_map:
        try:
            synth = int(key)
        except Exception:
            continue
        if synth < 1 or synth > 16:
            continue
        name = str(synth_map.get(key, "")).strip()
        if name and name.lower().endswith(".patch"):
            name = name[:-6]

        dirty_path = _env_dir + "/" + str(synth) + ".dirty"
        patch_path = _env_dir + "/" + name + ".patch" if name else None
        source_path = None
        is_dirty = False
        if _path_exists(dirty_path):
            source_path = dirty_path
            is_dirty = True
        elif patch_path and _path_exists(patch_path):
            source_path = patch_path

        if source_path is None:
            continue

        amy.send_raw("i%dic255" % (synth))
        load_patch_file(source_path)
        result["loaded_channels"].append(synth)
        if is_dirty:
            result["dirty_channels"].append(synth)

    return result


def environment_transfer_done(*_args):
    import os
    import machine
    tulip.stderr_write("Environment transfer done hook called")

    # Create the folders if they don't exist (and also empty files)
    _ensure_current_env_layout() 

    user_base = tulip.root_dir() + "user"
    current_base = user_base + "/current"
    env_dir = current_base

    env_tar = env_dir + "/environment.tar"
    incoming_tar = user_base + "/__incoming_environment.tar"
    cwd = os.getcwd().rstrip("/")
    cwd_tar = os.getcwd().rstrip("/") + "/environment.tar"
    source_candidates = ["environment.tar", cwd_tar, current_base + "/environment.tar", env_tar]
    source_tar = None
    for candidate in source_candidates:
        if _path_exists(candidate):
            source_tar = candidate
            break
    if source_tar is None:
        tulip.stderr_write("environment transfer done hook: environment.tar not found\n")
        return
    if source_tar == "environment.tar":
        source_tar = cwd + "/environment.tar"

    try:
        os.remove(incoming_tar)
    except OSError:
        pass

    os.chdir(user_base)

    try:
        os.rename(source_tar, incoming_tar)
    except OSError:
        tulip.stderr_write("environment transfer done hook: could not stage environment.tar in /user\n")
        return

    _clear_directory(env_dir)

    try:
        os.rename(incoming_tar, env_tar)
    except OSError:
        tulip.stderr_write("environment transfer done hook: could not move environment.tar into /user/current\n")
        return

    os.chdir(env_dir)
    tulip.tar_extract("environment.tar", show_progress=False)
    try:
        os.remove("environment.tar")
    except OSError:
        pass
    
    tulip.stderr_write("Resetting AMYboard")
    time.sleep(1)
    machine.reset()


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
    _env_dir = _ensure_current_env_layout()
    restore_patch_state_from_files(_env_dir, send_default_if_missing=True)

    from upysh import cd
    tulip.stderr_write("cding to %s" % (_env_dir))
    cd(_env_dir)
    try:
        from machine import Pin
        button = Pin(0, Pin.IN, Pin.PULL_UP)
        if button.value() != 0:  # Skip env.py if boot button held
            tulip.stderr_write("execfile env.py")
            execfile("env.py")
    except Exception as e:
        print("Environment start failed:")
        sys.print_exception(e)
    tulip.stderr_write("cding back to %s" % (tulip.root_dir() + "user"))
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
    """Legacy helper — calls display.show() if a display exists."""
    if display is not None:
        display.show()

def display_startup():
    display.text("AMYboard!!!!", 0, 0, 255)
    display.text(tulip.version(), 0, 24, 255)
    display.show()

def init_display():
    global display
    display = Display()
    if display.available:
        display_startup()

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
    return tulip.cv_in(channel)

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


# Patch Selector -
# If you have a display and a rotary encoder on I2C
# this function will let you scroll through your available patches
# and load each one by clicking the rotary encoder.
#
# You can launch it from env.py like this:
#
#   amyboard.patch_selector()
#
# or, for the 4-knob Adafruit rotary encoder:
#
#   amyboard.patch_selector(seesaw_dev=0x49, button_pin=12)

class PatchSelector:

    def __init__(
            self, synth=1, seesaw_dev=0x36, encoder=0, button_pin=24,
            patch_dir='/user/current', extension='.patch',
            hold_max=10, exit_callback=lambda: True
    ):
        """Endless loop scrolling through patch files and installing on click."""
        self.synth = synth
        self.seesaw_dev = seesaw_dev
        self.encoder = encoder
        self.button_pin = button_pin
        self.patch_dir = patch_dir
        self.extension = extension
        self.hold_max = hold_max
        self.exit_callback = exit_callback
        # State
        self.patches = self._list_patches()
        if not self.patches:
            raise ValueError('No .patch files found in ' + self.patch_dir)
        self.index = 0  # within patches
        self.button_state = False
        self.hold_count = 0
        # Initialize
        display.clear()
        display.message("PATCH SELECTOR", 0)
        init_buttons(pins=(self.button_pin,), seesaw_dev=self.seesaw_dev)

    def _list_patches(self):
        """List of the patch files."""
        files = [f[:-len(self.extension)] for f in os.listdir(self.patch_dir) if f.endswith(self.extension)]
        return files

    def update(self):
        index = read_encoder(self.encoder, seesaw_dev=self.seesaw_dev) % len(self.patches)
        button_state = read_buttons(pins=(self.button_pin,), seesaw_dev=self.seesaw_dev)[0]
        if (index, button_state) != (self.index, self.button_state):
            # Only rewrite display if it's out of sync.
            display.message(self.patches[index], 2, inverse=button_state)
        if button_state and not self.button_state:
            # Button transitioned to down
            load_patch_file(self.patch_dir + '/' + self.patches[index] + self.extension, synth=self.synth)
        self.hold_count = self.hold_count + 1 if button_state else 0
        if self.hold_count > self.hold_max:
            self.exit_callback()
        self.index = index
        self.button_state = button_state


def patch_selector(synth=1, duration=30, seesaw_dev=0x36, encoder=0, button_pin=24,
                   patch_dir='/user/current', extension='.patch'):
    """Run the PATCH SELECTOR app using Tulip sequencer."""
    import sequencer
    import machine
    seq = None
    
    def exit_callback():
        # Terminate the sequencing
        if seq:
            # Stop sequencer
            seq.clear()
            # Clear screen
            display.clear()
    # Initialize the patch selector
    patchsel_obj = PatchSelector(synth=synth, seesaw_dev=seesaw_dev, encoder=encoder, button_pin=button_pin,
                                 patch_dir=patch_dir, extension=extension, exit_callback=exit_callback)
    # Arrange for it to be called every 32nd note
    seq = sequencer.TulipSequence(32, lambda x: patchsel_obj.update())

