# © 2022 Václav Šmilauer <eu@doxos.eu>
# MIT-licensed

import time
import machine
import struct
import uctypes

from micropython import const

# This driver was written from scratch using datasheets and looking at other drivers listed here.
# Required copyright notices of those drivers are included below as necessary.

# This is Pimoroni driver, with Adafruit header (MIT, notice included below):
# https://github.com/pimoroni/st7789-python/blob/master/library/ST7789/__init__.py
# This is c++ Adafruit driver (MIT, notice included below):
# https://github.com/adafruit/Adafruit-ST7735-Library/blob/master/Adafruit_ST7789.cpp
# independent (?) micropython implementation (license unspecified):
# https://techatronic.com/st7789-display-pi-pico/
# st77xx c driver (for uPy), with simplified init sequence (MIT, notice included below):
# https://github.com/szampardi/st77xx_mpy


#
# This is a library for several Adafruit displays based on ST77* drivers.
# 
#   Works with the Adafruit 1.8" TFT Breakout w/SD card
#     ----> http://www.adafruit.com/products/358
#   The 1.8" TFT shield
#     ----> https://www.adafruit.com/product/802
#   The 1.44" TFT breakout
#     ----> https://www.adafruit.com/product/2088
#   as well as Adafruit raw 1.8" TFT display
#     ----> http://www.adafruit.com/products/618
#
# Check out the links above for our tutorials and wiring diagrams.
# These displays use SPI to communicate, 4 or 5 pins are required to
# interface (RST is optional).
#
# Adafruit invests time and resources providing this open source code,
# please support Adafruit and open-source hardware by purchasing
# products from Adafruit!
#
# Written by Limor Fried/Ladyada for Adafruit Industries.
# MIT license, all text above must be included in any redistribution.
#


#
# Copyright (c) 2019 Ivan Belokobylskiy
#

#
# Copyright (c) 2014 Adafruit Industries
# Author: Tony DiCola
#

#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.


ST77XX_NOP = const(0x00)
ST77XX_SWRESET = const(0x01)
ST77XX_RDDID = const(0x04)
ST77XX_RDDST = const(0x09)

ST77XX_SLPIN = const(0x10)
ST77XX_SLPOUT = const(0x11)
ST77XX_PTLON = const(0x12)
ST77XX_NORON = const(0x13)

ST77XX_INVOFF = const(0x20)
ST77XX_INVON = const(0x21)
ST77XX_DISPOFF = const(0x28)
ST77XX_DISPON = const(0x29)

ST77XX_CASET = const(0x2A)
ST77XX_RASET = const(0x2B)
ST77XX_RAMWR = const(0x2C)
ST77XX_RAMRD = const(0x2E)

ST77XX_PTLAR = const(0x30)
ST77XX_MADCTL = const(0x36)
ST77XX_COLMOD = const(0x3A)

ST7789_WRCACE = const(0x55)

ST77XX_FRMCTR1 = const(0xB1)
ST77XX_FRMCTR2 = ST7789_PORCTRL = const(0xB2)
ST77XX_FRMCTR3 = const(0xB3)
ST77XX_INVCTR = const(0xB4)
ST7789_DISSET5 = const(0xB6)

ST7789_GCTRL = const(0xB7)
ST7789_GTADJ = const(0xB8)
ST7789_VCOMS = const(0xBB)

ST7735_PWCTR1 = ST7789_LCMCTRL = const(0xC0)
ST7735_PWCTR2 = ST7789_IDSET = const(0xC1)
ST7735_PWCTR3 = ST7789_VDVVRHEN = const(0xC2)
ST7735_PWCTR4 = ST7789_VRHS = const(0xC3)
ST7735_PWCTR5 = ST7789_VDVS = const(0xC4)
ST7735_VMCTR1 = ST7789_VMCTR1 = const(0xC5)
ST7789_FRCTRL2 = const(0xC6)
ST7789_CABCCTRL = const(0xC7)

ST7789_PWCTRL1 = const(0xD0)
ST77XX_RDID1 = const(0xDA)
ST77XX_RDID2 = const(0xDB)
ST77XX_RDID3 = const(0xDC)
ST77XX_RDID4 = const(0xDD)

ST7789_GMCTRP1 = ST7789_PVGAMCTRL = const(0xE0)
ST7789_GMCTRN1 = ST7789_NVGAMCTRL = const(0xE1)

ST7735_PWCTR6 = ST7789_PWCTR6 = const(0xFC)


ST77XX_MADCTL_MY = const(0x80)  # page address order (0: top to bottom; 1: bottom to top)
ST77XX_MADCTL_MX = const(0x40)  # column address order (0: left to right; 1: right to left)
ST77XX_MADCTL_MV = const(0x20)  # page/column order (0: normal mode 1; reverse mode)
ST77XX_MADCTL_ML = const(0x10)  # line address order (0: refresh to to bottom; 1: refresh bottom to top)
ST77XX_MADCTL_BGR = const(0x08) # colors are BGR (not RGB)
ST77XX_MADCTL_RTL = const(0x04) # refresh right to left
ST77XX_MADCTL_ROTS=(
    const(0x00),                                # 0 = portrait
    const(ST77XX_MADCTL_MX | ST77XX_MADCTL_MV), # 1 = landscape
    const(ST77XX_MADCTL_MY | ST77XX_MADCTL_MX), # 2 = inverted portrait
    const(ST77XX_MADCTL_MY | ST77XX_MADCTL_MV), # 3 = inverted landscape
)

ST77XX_COLOR_MODE_65K = const(0x50)
ST77XX_COLOR_MODE_262K = const(0x60)
ST77XX_COLOR_MODE_12BIT = const(0x03)
ST77XX_COLOR_MODE_16BIT = const(0x05)
ST77XX_COLOR_MODE_18BIT = const(0x06)
ST77XX_COLOR_MODE_16M = const(0x07)


ST77XX_COL_ROW_MODEL_START_ROTMAP={
    # ST7789
    (240,320,None):[(0,0),(0,0),(0,0),(0,0)],
    (240,240,None):[(0,0),(0,0),(0,80),(80,0)],
    (135,240,None):[(52,40),(40,53),(53,40),(40,52)],
    # ST7735
    (128,160,'blacktab'):[(0,0),(0,0),(0,0),(0,0)],
    (128,160,'redtab'):[(2,1),(1,2),(2,1),(1,2)],
}

ST77XX_PORTRAIT = const(0)
ST77XX_LANDSCAPE = const(1)
ST77XX_INV_PORTRAIT = const(2)
ST77XX_INV_LANDSCAPE = const(3)

class St77xx_hw(object):
    def __init__(self, *, cs, dc, spi, res, suppRes, bl=None, model=None, suppModel=[], rst=None, rot=ST77XX_LANDSCAPE, bgr=False, rp2_dma=None):
        '''
        This is an abstract low-level driver the ST77xx controllers, not to be instantiated directly.
        Derived classes implement chip-specific bits. THe following parameters are recognized:

        * *cs*: chip select pin (= slave select, SS)
        * *dc*: data/command pin
        * *bl*: backlight PWM pin (optional)
        * *model*: display model, to account for variations in products
        * *rst*: optional reset pin
        * *res*: resolution tuple; (width,height) with zero rotation
        * *rot*: display orientation (0: portrait, 1: landscape, 2: inverted portrait, 3: inverted landscape); the constants ST77XX_PORTRAIT, ST77XX_LANDSCAPE, ST77XX_INV_POTRAIT, ST77XX_INV_LANDSCAPE may be used.
        * *bgr*: color order if BGR (not RGB)
        * *rp2_dma*: optional DMA object for the rp2 port


        Subclass constructors (implementing concrete chip) set in addition the following, not to be used directly:

        * *suppModel*: models supported by the hardware driver
        * *suppRes*: resolutions supported by the hardware driver, as list of (width,height) tuples
        '''
        self.buf1 = bytearray(1)
        self.buf2 = bytearray(2)
        self.buf4 = bytearray(4)

        self.cs,self.dc,self.rst=[(machine.Pin(p,machine.Pin.OUT) if isinstance(p,int) else p) for p in (cs,dc,rst)]
        self.bl=bl
        if isinstance(self.bl,int): self.bl=machine.PWM(machine.Pin(self.bl,machine.Pin.OUT))
        elif isinstance(self.bl,machine.Pin): self.bl=machine.PWM(self.bl)
        assert isinstance(self.bl,(machine.PWM,type(None)))
        self.set_backlight(10) # set some backlight

        self.rot=rot
        self.bgr=bgr
        self.width,self.height=(0,0) # this is set later in hard_reset->config->apply_rotation

        if res not in suppRes: raise ValueError('Unsupported resolution %s; the driver supports: %s.'%(str(res),', '.join(str(r) for r in suppRes)))
        if suppModel and model not in suppModel: raise ValueError('Unsupported model %s; the driver supports: %s.'%(str(model),', '.join(str(r) for r in suppModel)))

        self.res=res
        self.model=model

        self.rp2_dma=rp2_dma
        self.spi=spi
        self.hard_reset()


    def off(self): self.set_backlight(0)

    def hard_reset(self):
        if self.rst:
            for v in (1,0,1):
                self.rst.value(v)
                time.sleep(.2)
            time.sleep(.2)
        self.config()
    def config(self):
        self.config_hw() # defined in child classes
        self.apply_rotation(self.rot)
    def set_backlight(self,percent):
        if self.bl is None: return
        self.bl.duty_u16(percent*655)
    def set_window(self, x, y, w, h):
        c0,r0=ST77XX_COL_ROW_MODEL_START_ROTMAP[self.res[0],self.res[1],self.model][self.rot%4]
        struct.pack_into('>hh', self.buf4, 0, c0+x, c0+x+w-1)
        self.write_register(ST77XX_CASET, self.buf4)
        struct.pack_into('>hh', self.buf4, 0, r0+y, r0+y+h-1)
        self.write_register(ST77XX_RASET, self.buf4)

    def apply_rotation(self,rot):
        self.rot=rot
        if (self.rot%2)==0: self.width,self.height=self.res
        else: self.height,self.width=self.res
        self.write_register(ST77XX_MADCTL,bytes([(ST77XX_MADCTL_BGR if self.bgr else 0)|ST77XX_MADCTL_ROTS[self.rot%4]]))

    def blit(self, x, y, w, h, buf, is_blocking=True):
        self.set_window(x, y, w, h)
        if self.rp2_dma: self._rp2_write_register_dma(ST77XX_RAMWR, buf, is_blocking)
        else: self.write_register(ST77XX_RAMWR, buf)

    def clear(self, color):
        bs=128 # write pixels in chunks; makes the fill much faster
        struct.pack_into('>h',self.buf2,0,color)
        buf=bs*bytes(self.buf2)
        npx=self.width*self.height
        self.set_window(0, 0, self.width, self.height)
        self.write_register(ST77XX_RAMWR, None)
        self.cs.value(0)
        self.dc.value(1)
        for _ in range(npx//bs): self.spi.write(buf)
        for _ in range(npx%bs): self.spi.write(self.buf2)
        self.cs.value(1)

    def write_register(self, reg, buf=None):
        struct.pack_into('B', self.buf1, 0, reg)
        self.cs.value(0)
        self.dc.value(0)
        self.spi.write(self.buf1)
        if buf is not None:
            self.dc.value(1)
            self.spi.write(buf)
        self.cs.value(1)

    def _rp2_write_register_dma(self, reg, buf, is_blocking=True):
        'If *is_blocking* is False, used should call wait_dma explicitly.'
        SPI1_BASE = 0x40040000 # FIXME: will be different for another SPI bus?
        SSPDR     = 0x008
        self.rp2_dma.config(
            src_addr = uctypes.addressof(buf),
            dst_addr = SPI1_BASE + SSPDR,
            count    = len(buf),
            src_inc  = True,
            dst_inc  = False,
            trig_dreq= self.rp2_dma.DREQ_SPI1_TX
        )
        struct.pack_into('B',self.buf1,0,reg)
        self.cs.value(0)

        self.dc.value(0)
        self.spi.write(self.buf1)

        self.dc.value(1)
        self.rp2_dma.enable()

        if is_blocking: self.rp2_wait_dma()

    def rp2_wait_dma(self):
        '''
        Wait for rp2-port DMA transfer to finish; no-op unless self.rp2_dma is defined.
        Can be used as callback before accessing shared SPI bus e.g. with the xpt2046 driver.
        '''
        if self.rp2_dma is None: return
        while self.rp2_dma.is_busy(): pass
        self.rp2_dma.disable()
        # wait to send last byte. It should take < 1uS @ 10MHz
        time.sleep_us(1)
        self.cs.value(1)

    def _run_seq(self,seq):
        '''
        Run sequence of (initialization) commands; those are given as list of tuples, which are either
        `(command,data)` or `(command,data,delay_ms)`
        '''
        for i,cmd in enumerate(seq):
            if len(cmd)==2: (reg,data),delay=cmd,0
            elif len(cmd)==3: reg,data,delay=cmd
            else: raise ValueError('Command #%d has %d items (must be 2 or 3)'%(i,len(cmd)))
            self.write_register(reg,data)
            if delay>0: time.sleep_ms(delay)


class St7735_hw(St77xx_hw):
    '''There are several ST7735-based LCD models, we only tested the blacktab model really.'''
    def __init__(self,res,model='greentab',**kw):
        super().__init__(res=res,suppRes=[(128,160),],model=model,suppModel=['greentab','redtab','blacktab'],**kw)
    def config_hw(self):
        # mostly from here
        # https://github.com/stechiez/raspberrypi-pico/blob/main/pico_st7735/st7735/ST7735.py

        init7735r=[
            # see here for explanations: https://github.com/adafruit/Adafruit-ST7735-Library/blob/master/Adafruit_ST7735.cpp
            (ST77XX_SWRESET,None, 50),
            (ST77XX_SLPOUT, None, 100),
            (ST77XX_FRMCTR1,b'\x01\x2c\x2d'),
            (ST77XX_FRMCTR2,b'\x01\x2c\x2d'),
            (ST77XX_FRMCTR3,b'\x01\x2c\x2d\x01\x2c\x2d'),
            (ST77XX_INVCTR,b'\x07'),
            (ST7735_PWCTR1,b'\xa2\x02\xb4'),
            (ST7735_PWCTR2,b'\xc5'),
            (ST7735_PWCTR3,b'\x0a\x00'),
            (ST7735_PWCTR4,b'\x8a\x2a'),
            (ST7735_PWCTR5,b'\x8a\xee'),
            (ST7735_VMCTR1,b'\x0e'),
            (ST77XX_INVOFF,None),
            # ST77XX_MADCTL: do later, depending on rotation
            (ST77XX_COLMOD,bytes([ST77XX_COLOR_MODE_65K | ST77XX_COLOR_MODE_16BIT])),
            (ST77XX_CASET,bytes([0x00,0x00,0x00,0x7f])),
            (ST77XX_RASET,bytes([0x00,0x00,0x00,0x9f])),
            # gamma adjustment: Waveshare values
            (ST7789_GMCTRP1,b'\x0f\x1a\x0f\x18\x2f\x28\x20\x22\x1f\x1b\x23\x37\x00\x07\x02\x10'),
            (ST7789_GMCTRN1,b'\x0f\x1b\x0f\x17\x33\x2c\x29\x2e\x30\x30\x39\x3f\x00\x07\x03\x10'),
            (ST77XX_NORON, None, 10),
            (ST77XX_DISPON, None,100),
        ]

        # the "blue version" only (not tested)
        init7735=[
            # swreset
            (ST77XX_SWRESET, None, 50),
            # out of sleep mode
            (ST77XX_SLPOUT, None, 100),
            # RGB565
            (ST77XX_COLMOD,bytes([ST77XX_COLOR_MODE_65K | ST77XX_COLOR_MODE_16BIT])),
            # fast refresh (??)
            (ST77XX_FRMCTR1,bytes([0x00,0x06,0x03])),
            (ST77XX_MADCTL,bytes([0x03])),
            (ST77XX_INVCTR,b'\x00'),
            (ST7735_PWCTR1,b'\x02\x70'),
            (ST7735_PWCTR2,b'\x05'),
            (ST7735_PWCTR3,b'\x01\x02'),
            (ST7735_VMCTR1,b'\x3c\x38'),
            (ST7735_PWCTR6,b'\b11\b15'),
            # (ST77XX_GMCTRP1,b'\
            ## memory access direction
            # (ST77XX_MADCTL, bytes([ST77XX_MADCTL_ROTS[self.rot%4]]), 0),
            # inverted on (?)
            #(ST77XX_INVON, None, 10),
            # normal display on
            (ST77XX_NORON, None, 10),
            # display on
            (ST77XX_DISPON, None,100)
        ]
        if self.model in ('redtab','blacktab'): self._run_seq(init7735r)
        else:
            print('Warning: the greentab model was never properly tested')
            self._run_seq(init7735)
        # ST77XX_MADCTL applied in apply_rotation


class St7789_hw(St77xx_hw):
    def __init__(self,res,**kw):
        super().__init__(res=res,suppRes=[(240,320),],model=None,suppModel=None,**kw)
    def config_hw(self):
        init7789=[
            # out of sleep mode
            (ST77XX_SLPOUT, None, 100),
            # memory access direction (this is set again in apply_rotation)
            (ST77XX_MADCTL, bytes([ST77XX_MADCTL_ROTS[self.rot%4]])),
            # RGB565
            (ST77XX_COLMOD, bytes([ST77XX_COLOR_MODE_65K | ST77XX_COLOR_MODE_16BIT])),
            # front/back porch setting in normal/idle/partial modes; 3rd byte (PSEN) 0x00 = disabled
            (ST7789_PORCTRL, b"\x0C\x0C\x00\x33\x33"),
            # VGH=14.06V, VGL=-8.87V [Adafruit: 0x14]
            (ST7789_GCTRL, b"\x35"),
            # [Adafruit: missing]
            (ST77XX_DISPOFF, b"\x28"),
            # power control [Adafruit: 0x2c]
            (ST7789_LCMCTRL, b"\x3C"),
            # power control (set VDV and VRD by register write), write VRH and VDV
            (ST7789_VDVVRHEN, b"\x01"),(ST7789_VRHS, b"\x0B"),(ST7789_VDVS, b"\x20"),
            # frame rate 60Hz
            (ST7789_FRCTRL2, b"\x0F"),
            # power control: AVDD=6.6V, AVCL=-4.8V, VDS=2.4V
            (ST7789_PWCTRL1, b"\xA4\xA1"),
            # positive voltage gamma control
            (ST7789_PVGAMCTRL, b"\xD0\x01\x08\x0F\x11\x2A\x36\x55\x44\x3A\x0B\x06\x11\x20"),
            # negative voltage gamma control
            (ST7789_NVGAMCTRL, b"\xD0\x02\x07\x0A\x0B\x18\x34\x43\x4A\x2B\x1B\x1C\x22\x1F"),
            # content adaptive brightness control and color enhancement: color enhancement on, high enhancement
            (ST7789_WRCACE, bytes([0b1011_0000])),
            # display on
            (ST77XX_DISPON, None, 100),
            ## FIXME: needs out of sleep mode AGAIN, otherwise will stay bleck the first time on?
            (ST77XX_SLPOUT, None, 100),
        ]
        self._run_seq(init7789)
        # ST77XX_MADCTL applied in apply_rotation

class St77xx_lvgl(object):
    '''LVGL wrapper for St77xx, not to be instantiated directly.

    * creates and registers LVGL display driver;
    * allocates buffers (double-buffered by default);
    * sets the driver callback to the disp_drv_flush_cb method.

    '''
    def disp_drv_flush_cb(self,disp_drv,area,color_p):
        self.rp2_wait_dma() # wait if not yet done and DMA is being used
        
        w = area.x2 - area.x1 + 1
        h = area.y2 - area.y1 + 1
        size = w * h
        data_view = color_p.__dereference__(size * self.pixel_size)
        if self.rgb565_swap_func:
            self.rgb565_swap_func(data_view, size)
        
        # blit in background
        self.blit(area.x1, area.y1, w, h, data_view, is_blocking=False)
        self.disp_drv.flush_ready()
    
    def __init__(self,doublebuffer=True,factor=4):
        import lvgl as lv
        import lv_utils

        color_format = lv.COLOR_FORMAT.RGB565
        self.pixel_size = lv.color_format_get_size(color_format)
        self.rgb565_swap_func = None if self.bgr else lv.draw_sw_rgb565_swap

        if not lv.is_initialized(): lv.init()

        # create event loop if not yet present
        if not lv_utils.event_loop.is_running(): self.event_loop=lv_utils.event_loop()

        # create display buffer(s)
        draw_buf1 = lv.draw_buf_create(self.width, self.height // factor, color_format, 0)
        draw_buf2 = lv.draw_buf_create(self.width, self.height // factor, color_format, 0) if doublebuffer else None
        
        # attach all to self to avoid objects' refcount dropping to zero when the scope is exited
        self.disp_drv = lv.display_create(self.width, self.height)
        self.disp_drv.set_color_format(color_format)
        self.disp_drv.set_draw_buffers(draw_buf1, draw_buf2)
        self.disp_drv.set_render_mode(lv.DISPLAY_RENDER_MODE.PARTIAL)
        self.disp_drv.set_flush_cb(self.disp_drv_flush_cb)

class St7735(St7735_hw,St77xx_lvgl):
    def __init__(self,res,doublebuffer=True,factor=4,**kw):
        '''See :obj:`St77xx_hw` for the meaning of the parameters.'''
        St7735_hw.__init__(self,res=res,**kw)
        St77xx_lvgl.__init__(self,doublebuffer,factor)

class St7789(St7789_hw,St77xx_lvgl):
    def __init__(self,res,doublebuffer=True,factor=4,**kw):
        '''See :obj:`St77xx_hw` for the meaning of the parameters.'''
        St7789_hw.__init__(self,res=res,**kw)
        St77xx_lvgl.__init__(self,doublebuffer,factor)
