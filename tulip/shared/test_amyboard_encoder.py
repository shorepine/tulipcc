"""Host-side test of amyboard.py's encoder detection against a fake I2C bus.

amyboard.py is frozen into the firmware, so this can't be exercised on device
without the hardware in hand. Instead it stubs the firmware modules amyboard.py
imports and drives _detect_encoder_devices()/Encoder against a register-level
fake of the seesaw and M5Stack accessories -- enough to pin down which board is
recognized at which address, and which registers get written to it.

Run from anywhere:  python3 tulip/shared/test_amyboard_encoder.py
"""
import os, sys, struct
from unittest import mock

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "amyboard-py"))
for name in ("tulip", "midi", "amy"):
    sys.modules[name] = mock.MagicMock(name=name)
sys.modules["amy"].AMY_SEQUENCER_PPQ = 48
sys.modules["tulip"].board = lambda: "AMYBOARD"

import amyboard

SEESAW_HW_ID = 0x87  # ATtiny817


class FakeI2C:
    """Minimal seesaw/M5 register-level fake. devices: {addr: product_id or 'other'}"""
    def __init__(self, devices, encoder_pos=None):
        self.devices = devices
        self.encoder_pos = encoder_pos or {}
        self.pending = {}
        self.writes = []          # every (addr, bytes) written

    def scan(self):
        return sorted(self.devices)

    def writeto(self, addr, data):
        if addr not in self.devices:
            raise OSError("no device at 0x%02x" % addr)
        self.writes.append((addr, bytes(data)))
        self.pending[addr] = bytes(data)

    def writeto_mem(self, addr, reg, data):
        self.writes.append((addr, bytes([reg]) + bytes(data)))

    def _response(self, addr, n):
        req = self.pending.get(addr, b"")
        prod = self.devices[addr]
        if prod == "other":               # not a seesaw: garbage HW ID
            return bytes([0x12] * n)
        if req[:2] == bytes([0x00, 0x01]):           # STATUS_HW_ID
            return bytes([SEESAW_HW_ID]) + bytes(n - 1)
        if req[:2] == bytes([0x00, 0x02]):           # STATUS_VERSION
            return struct.pack(">I", (prod << 16) | 1)[:n]
        if req[:1] == bytes([0x11]):                 # ENCODER_POSITION + n
            which = req[1] - 0x30
            return struct.pack(">i", self.encoder_pos.get((addr, which), 0))
        if req[:2] == bytes([0x01, 0x04]):           # GPIO bulk: all high (no press)
            return b"\xff\xff\xff\xff"
        return bytes(n)

    def readfrom(self, addr, n):
        if addr not in self.devices:
            raise OSError("no device")
        return self._response(addr, n)

    def readfrom_into(self, addr, buf):
        buf[:] = self._response(addr, len(buf))


def use(devices, **kw):
    bus = FakeI2C(devices, **kw)
    amyboard.get_i2c = lambda: bus
    amyboard._seesaw_missing.clear()
    return bus


fails = []
def check(label, got, want):
    ok = got == want
    print(("  ok   " if ok else "  FAIL ") + "%s: %r%s" % (label, got, "" if ok else " (want %r)" % (want,)))
    if not ok:
        fails.append(label)

print("1. ANO alone at 0x49 (product 5740)")
bus = use({0x49: 5740})
enc = amyboard.encoder()
check("devices", enc.devices, [("adafruit_ano", 0x49)])
check("encoders", enc.encoders, 1)
check("buttons", enc.buttons, 5)
check("leds", enc.leds, 0)
check("no neopixel writes", [w for w in bus.writes if w[1][:1] == bytes([0x0E])], [])
check("pullups on pins 1-5",
      any(w[1][:2] == bytes([0x01, 0x0B]) and struct.unpack(">I", w[1][2:])[0] == 0b111110
          for w in bus.writes), True)
check("button(ANO_UP) not pressed", enc.button(amyboard.ANO_UP), False)
check("button(5) out of range", enc.button(5), False)

print("2. quad alone at 0x49 (product 5752) still behaves as before")
bus = use({0x49: 5752}, encoder_pos={(0x49, 2): 7})
enc = amyboard.encoder()
check("devices", enc.devices, [("adafruit_quad", 0x49)])
check("encoders/buttons/leds", (enc.encoders, enc.buttons, enc.leds), (4, 4, 4))
check("neopixels initialized", any(w[1][:2] == bytes([0x0E, 0x01]) for w in bus.writes), True)
bus.encoder_pos[(0x49, 2)] = 10
check("read(2) relative to construction", enc.read(2), 3)

print("3. single at 0x36 (product 4991)")
bus = use({0x36: 4991})
enc = amyboard.encoder()
check("devices", enc.devices, [("adafruit_single", 0x36)])
check("encoders/buttons/leds", (enc.encoders, enc.buttons, enc.leds), (1, 1, 1))

print("4. quad 0x49 + ANO 0x4A + single 0x36 together")
use({0x49: 5752, 0x4A: 5740, 0x36: 4991})
enc = amyboard.encoder()
check("devices", enc.devices,
      [("adafruit_quad", 0x49), ("adafruit_ano", 0x4A), ("adafruit_single", 0x36)])
check("encoders", enc.encoders, 4 + 1 + 1)
check("buttons", enc.buttons, 4 + 5 + 1)
check("leds", enc.leds, 4 + 0 + 1)
check("type", enc.type, "multi")
check("ANO select is button 4", enc._button_map[4][0]["type"], "adafruit_ano")
check("single's button is last", enc._button_map[9][0]["type"], "adafruit_single")

print("5. unknown product ID at 0x49 falls back to quad (back-compat)")
use({0x49: 1234})
check("devices", amyboard.encoder().devices, [("adafruit_quad", 0x49)])

print("6. unknown product ID at 0x55 -> ANO range")
use({0x55: 1234})
check("devices", amyboard.encoder().devices, [("adafruit_ano", 0x55)])

print("7. non-seesaw device (e.g. ADS1115) at 0x49 ignored")
use({0x49: "other"})
check("devices", amyboard.encoder().devices, [])

print("8. exclude= skips an address")
use({0x49: 5740, 0x36: 4991})
check("devices", amyboard.encoder(exclude=(0x49,)).devices, [("adafruit_single", 0x36)])

print("9. forced type= without detection")
use({})
enc = amyboard.encoder(type="adafruit_ano", addr=0x4B)
check("devices", enc.devices, [("adafruit_ano", 0x4B)])
check("buttons", enc.buttons, 5)

print("10. register_seesaw_device() adds a board at runtime")
amyboard.register_seesaw_device("my_board", product_id=4242, addrs=range(0x60, 0x62),
                                encoders=2, button_pins=(1, 2), leds=1, neopixel_pin=9)
use({0x60: 4242})
enc = amyboard.encoder()
check("devices", enc.devices, [("my_board", 0x60)])
check("encoders/buttons/leds", (enc.encoders, enc.buttons, enc.leds), (2, 2, 1))

print("11. M5Stack 8Encoder unaffected")
use({0x41: "other"})
enc = amyboard.encoder()
check("devices", enc.devices, [("m5stack", 0x41)])
check("encoders/buttons", (enc.encoders, enc.buttons), (8, 8))

print("12. AMYboard's own CV chips are never probed (ADS1015 0x48, GP8413 0x58)")
bus = use({0x48: "other", 0x58: "other", 0x36: 4991})
enc = amyboard.encoder()
check("devices", enc.devices, [("adafruit_single", 0x36)])
check("no writes to CV chips", [w for w in bus.writes if w[0] in (0x48, 0x58)], [])

print("13. no encoder hardware at all")
use({})
enc = amyboard.encoder()
check("encoders/buttons/leds", (enc.encoders, enc.buttons, enc.leds), (0, 0, 0))
check("read/button safe", (enc.read(0), enc.button(0), enc.switch()), (0, False, False))

print()
print("FAILURES: %d" % len(fails) + ("" if not fails else " -> " + ", ".join(fails)))
sys.exit(1 if fails else 0)
