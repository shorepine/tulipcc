# © 2022 Václav Šmilauer <eu@doxos.eu>
# MIT-licensed
import machine
import struct

# for inspiration see e.g.
# https://github.com/MatthewLowden/RPi-XPT2046-Touchscreen-Python/blob/master/XPT2046.py
# and the XPT2046 datasheet

XPT2046_PORTRAIT = const(0)
XPT2046_LANDSCAPE = const(1)
XPT2046_INV_PORTRAIT = const(2)
XPT2046_INV_LANDSCAPE = const(3)


class Xpt2046_hw(object):
    CHAN_X  = const(0b0101_0000)
    CHAN_Y  = const(0b0001_0000)
    CHAN_Z1 = const(0b0011_0000)
    CHAN_Z2 = const(0b0100_0000)
    CHAN_T0 = const(0b0000_0000)
    CHAN_T1 = const(0b0111_0000)
    CHAN_BAT= const(0b0010_0000)
    CHAN_AUX= const(0b0110_0000)

    CONV_8_BIT =const(0b0000_1000)
    CONV_12_BIT=const(0b0000_0000)
    START_BIT  =const(0b1000_0000)

    def _chanRead(self,chan):
        self.cs.value(0)
        struct.pack_into('BBB',self.buf,0,Xpt2046.START_BIT|self.conv|chan,0,0)
        self.spi.write_readinto(self.buf,self.buf)
        if self.conv==Xpt2046.CONV_8_BIT: ret=self.buf[1]
        else: ret=(self.buf[1]<<4)|(self.buf[2]>>4)
        self.cs.value(1)
        return ret

    def __init__(self,*,
        spi: machine.SPI,cs,bits=12,ranges=((100,1900),(200,1950)),width=240,height=320,rot=XPT2046_PORTRAIT):
        '''
        Construct the Xpt2046 touchscreen controller.
        *spi*: spi bus instance; its baud rate must *not* exceed 2_000_000 (2MHz) for correct functionality
        *cs*: chip select (GPIO number or machine.Pin instance)
        *bits*: ADC precision, can be 12 or 8; note that 8 will require you to provide different *ranges*
        *ranges*: `(x_min,x_max),(y_min,y_max)` for raw coordinate readings; calibrated values might be provided.
        *width*: width of the underlying screen in pixels, in natural (rot=0) orientation (0..*width* is the range for reported horizontal coordinate)
        *height*: height of the underlying screen in pixels
        *rot*: screen rotation (0: portrait, 1: landscape, 2: inverted portrait, 3: inverted landscape); the constants XPT2046_PORTRAIT, XPT2046_LANDSCAPE, XPT2046_INV_PORTRAIT, XPT2046_INV_LANDSCAPE may be used.
        '''
        self.buf = bytearray(3)
        self.spi = spi
        self.cs  = (machine.Pin(cs,machine.Pin.OUT) if isinstance(cs,int) else cs)
        self.cs.value(1)
        if bits not in (8,12): raise ValueError('Xpt2046.bits: must be 8 or 12 (not %s)'%str(bits))
        self.conv=(Xpt2046.CONV_8_BIT if bits==8 else Xpt2046.CONV_12_BIT)
        self.xy_range,self.dim,self.rot=ranges,(width,height),(rot%4)
        self.xy_scale=[self.dim[ax]*1./(self.xy_range[ax][1]-self.xy_range[ax][0]) for ax in (0,1)]
        self.xy_origin=[self.xy_range[ax][0] for ax in (0,1)]
    def _raw2px(self,rxy):
        'Convert raw coordinates to pixel coordinates'
        x,y=[int(self.xy_scale[ax]*(rxy[ax]-self.xy_origin[ax])) for ax in (0,1)]
        if   self.rot==0: return self.dim[0]-x,y
        elif self.rot==1: return self.dim[1]-y,x
        elif self.rot==2: return x,self.dim[1]-y
        else:             return y,self.dim[0]-x
    def _raw_pos(self):
        'Read raw position; return value if within valid ranges (`__init__(ranges=...)`) or `None` if outside.'
        ret=[0,0]
        for ax,chan in [(0,Xpt2046.CHAN_X),(1,Xpt2046.CHAN_Y)]:
            r=self._chanRead(chan)
            if not self.xy_range[ax][0]<=r<=self.xy_range[ax][1]: return None
            ret[ax]=r
        return ret

    def pos(self,N=10,attempts=20):
        ''''
        Get N position readings (limited by 20 attempts) and return mean position of valid readings.
        If attempts are exhausted, return None.
        '''
        N,attempts=10,20
        xx,yy,done=0,0,0
        for _ in range(attempts):
            if (r:=self._raw_pos()) is None: continue
            xx+=r[0]; yy+=r[1]; done+=1
            if done==N: break
        else: return None
        mx,my=xx*1./N,yy*1./N
        return self._raw2px((mx,my))


class Xpt2046(Xpt2046_hw):
    def indev_drv_read_cb(self, indev_drv, data):
        # wait for DMA transfer (if any) before switchint SPI to 1 MHz
        if self.spiPrereadCb: self.spiPrereadCb()
        # print('.',end='')
        if self.spiRate: self.spi.init(baudrate=1_000_000)
        pos=self.pos()
        if pos is None: data.state=0
        else: (data.point.x,data.point.y),data.state=pos,1
        # print('#',end='')
        # switch SPI back to spiRate
        if self.spiRate: self.spi.init(baudrate=self.spiRate)

    def __init__(self,spi,spiRate=24_000_000,spiPrereadCb=None,**kw):
        '''XPT2046 touchscreen driver for LVGL; cf. documentation of :obj:`Xpt2046_hw` for the meaning of parameters being passed.

        *spiPrereadCb*: call this before reading from SPI; used to block until DMA transfer is complete (when sharing SPI bus).
        *spiRate*: the SPI bus must set to low frequency (1MHz) when reading from the XPT2046; when *spiRate* is given, the bus will be switched back to this frequency when XPT2046 is done reading. The default 24MHz targets St77xx display chips which operate at that frequency and come often with XPT2046-based touchscreen.
        '''
        super().__init__(spi=spi,**kw)
        self.spiRate=spiRate
        self.spiPrereadCb=spiPrereadCb

        import lvgl as lv
        if not lv.is_initialized(): lv.init()

        self.indev_drv = lv.indev_create()
        self.indev_drv.set_type(lv.INDEV_TYPE.POINTER)
        self.indev_drv.set_read_cb(self.indev_drv_read_cb)
