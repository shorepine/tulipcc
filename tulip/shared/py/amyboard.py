# amyboard.py
import tulip, midi, amy, time

i2c = None

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
    initpcm9211()
    time.sleep(1)
    tulip.start_amyboard_amy()
    time.sleep(1)
    amy.override_send = lambda x: tulip.amyboard_send(x)
    time.sleep(1)
    midi.setup()


def get_i2c():
    global i2c
    if(i2c is None):
        from machine import I2C
        i2c = I2C(0, freq=400000)
    return i2c

def adafruit_oled():
    import ssd1327
    display = ssd1327.SSD1327_I2C(128,128,get_i2c(),addr=0x3d)
    display.text("AMYboard!",0,0,255)
    display.show()
    return display

def sh1107_oled():
    import sh1107
    display = sh1107.SH1107_I2C(128, 128, get_i2c(), address=0x3c)
    display.sleep(False)
    display.fill(0)
    display.text('AMYboard!', 0, 0, 1)
    display.show()
    return display

def read_register(addr, reg):
    get_i2c().writeto(addr, bytes([reg]))
    b = get_i2c().readfrom(addr, 1)
    if(b):
        return int.from_bytes(b, "big")
    return None

def write_register(addr, reg, val):
    get_i2c().writeto(addr, bytes([reg, val]))

def initpcm9211(addr=0x40):
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

# TODO - keep this in C as "external output" in AMY
# TODO - should we take in -V and scale it ? 
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

# TODO - move this to C and have it be an AMY CtrlCoef input
def cv_in(channel=0, n=5):
    from machine import Pin, ADC
    pin = 15 if channel==1 else 16
    pot = ADC(Pin(pin))
    pot.atten(ADC.ATTN_11DB) 

    # Read it n times to smooth it
    x = 0
    for i in range(n):
        x = x + pot.read_uv()
    x = (x / (float(n))) 

    # measured uV with a CV pot module that goes from -5.4V to +5.4V (10vpp)
    (lo, hi) = 755000,2568000
    rge = hi-lo
    return (((x-lo) / rge) * 10.0) - 5.0
