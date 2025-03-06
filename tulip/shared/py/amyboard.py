# amyboard.py
import tulip, amy

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
    tulip.start_amyboard_amy()
    amy.override_send = lambda x: tulip.amyboard_send(x)
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
    addr = 89 # GP8413 - not working yet
    val = int((volts/10.0) * 65535.0)
    if(val>65535): val = 65535
    b1 = (val & 0xff00) >> 8
    b0 = (val & 0x00ff)
    ch = 0x02
    addr = 89
    if(channel == 1): ch = 0x04
    get_i2c().writeto_mem(addr, ch, bytes([b0,b1]))

# TODO - move this to C and have it be an AMY CtrlCoef input
def cv_in(channel=0, n=5):
    from machine import Pin, ADC
    pin = 16 if channel==1 else 15
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
