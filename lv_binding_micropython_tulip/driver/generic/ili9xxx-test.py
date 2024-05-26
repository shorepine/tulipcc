import time

from machine import SPI, Pin

from ili9xxx import Ili9341_hw


def build_rect_buf(w, h, inner=[0x00, 0x00]):
    top = b"\xFF\xFF" * w
    body = (b"\xFF\xFF\xFF" + bytes(inner) * (w - 3) + b"\xFF\xFF\xFF") * (h - 3)
    bot = b"\xFF\xFF" * w
    return top + body + bot


spi = SPI(
    0,
    baudrate=24_000_000,
    sck=Pin(18),
    mosi=Pin(19),
    miso=Pin(16),
)
lcd = Ili9341_hw(spi=spi, cs=17, dc=15, rst=14)

lcd.set_backlight(30)
for rot in (0, 1, 2, 3):
    lcd.apply_rotation(rot)

    lcd.clear(0x0000)
    # 1/4 screen pixels square with white border red background
    w, h = lcd.width // 4, lcd.height // 8
    bmp = build_rect_buf(w, h, [0x03, 0x03])
    t0 = time.ticks_us()
    lcd.blit(w, h, w, h, bmp)
    t1 = time.ticks_us()
    bmp = build_rect_buf(lcd.width, lcd.height // 20, [0x09, 0x09])
    lcd.blit(0, 0, lcd.width, lcd.height // 20, bmp)

    print("Maximum FPS @24MHz:", 24e6 / (320 * 240 * 16))  # FPS = F/(W*H*BPP)
    print(
        "Achieved FPS:", 1 / (16 * (t1 - t0) * 1e-6)
    )  # Note: Test only draws 1/16 of the sreen area

    print("Draw TSC calibration pattern")
    w, h, wu, hu = lcd.width // 10, lcd.height // 10, lcd.width // 5, lcd.height // 5
    bmp = build_rect_buf(w, h, [0xA0, 0xF0])
    lcd.blit(wu, hu, w, h, bmp)
    lcd.blit(4 * wu, hu, w, h, bmp)
    lcd.blit(4 * wu, 4 * hu, w, h, bmp)
    lcd.blit(wu, 4 * hu, w, h, bmp)
    time.sleep(0.5)
