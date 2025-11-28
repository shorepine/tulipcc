# amyboard.py
import tulip, midi, amy, time

i2c = None
display_buffer = None # the bytes object storing the display bits
display = None # The framebuf object to draw to 
hw_display = None # if there's an I2C display connected

def web():
    return (tulip.board()=="AMYBOARD_WEB")

def mount_sd():
    # mount the SD card if given
    import machine, uos
    try:
        sd = machine.SDCard(sck=12, miso=13, mosi=11, cs=10,slot=2)
        uos.mount(uos.VfsFat(sd), '/sd')
        print("SD card mounted as /sd")
    except OSError:
        pass # it's ok!

def start_amy():
    init_pcm9211()
    tulip.amyboard_start()

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
    display.text("AMYboard!",0,0,255)
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
# read_encoder(encoder=0)  # reads the current value of the encoder
# init_buttons()  # must be called to configure pullup of encoder buttons
# read_buttons()  # returns a boolean list of the state of the 4 buttons.

def read_encoder(encoder=0, seesaw_dev=0x49):
    """Read the cumulated value of encoder 0..3."""
    i2c = get_i2c()
    result = bytearray(4)
    ENCODER_BASE = 0x11
    ENCODER_POSITION = 0x30
    i2c.writeto(seesaw_dev, bytes([ENCODER_BASE, ENCODER_POSITION + encoder]))
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

def read_buttons(pins=(12, 14, 17, 9), seesaw_dev=0x49):
    """Read the 4 seesaw encoder push buttons."""
    delay = 0.008
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

    
