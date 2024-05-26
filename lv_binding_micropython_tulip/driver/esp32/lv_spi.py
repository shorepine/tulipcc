"""
MicroPython espidf SPI driver
Used, for example, to share spi bus between SPI screen and sdcard.py driver 

Provides simple init, deinit, write, read, readinto and write_readinto methods

Example usage with sdcard.py driver on ESP32 :
    
    import machine, os, lv_spi, sdcard
    spi = lv_spi.SPI(mosi=23, miso=38, clk=18)
    sd = sdcard.SDCard(spi, machine.Pin(4))
    os.mount(sd, "/sd")
    print(os.listdir('/sd'))

Example usage with ili93XXX.py + sdcard.py drivers (SPI bus sharing) :

    import machine, os, lv_spi, sdcard
    from ili9XXX import ili9341
    lcd = ili9341(mosi=23, miso=38, clk=18, dc=15, cs=5, invert=True, rot=0x10, width=320, height=240,
                  half_duplex=False) # half_duplex do not work with SDCard
    sd = sdcard.SDCard(lv_spi.SPI(), machine.Pin(4))
    os.mount(sd, "/sd")
    print(os.listdir('/sd'))

"""

import espidf as esp

class SPI(object):

    SPI_HOST = esp.SPI_HOST
    HSPI_HOST = esp.HSPI_HOST
    VSPI_HOST = esp.VSPI_HOST
    
    def __init__(self, spihost=HSPI_HOST, baudrate=200000, miso=-1, mosi=-1, clk=-1, cs=-1,
                 phase=0, polarity=0, half_duplex=False, max_transfer_sz=4092):
         
        self.spihost = spihost
        self.baudrate = baudrate
        self.miso = miso
        self.mosi = mosi
        self.clk = clk
        self.cs = cs
        self.phase = phase
        self.polarity = polarity
        self.half_duplex = half_duplex
        self.max_transfer_sz = max_transfer_sz        

        self.spi = None

        buscfg = esp.spi_bus_config_t(
            {
                "miso_io_num": self.miso,
                "mosi_io_num": self.mosi,
                "sclk_io_num": self.clk,
                "quadwp_io_num": -1,
                "quadhd_io_num": -1,
                "max_transfer_sz":  max_transfer_sz,
            }
        )

        # Initialize the SPI bus, if needed.
        if buscfg.miso_io_num >= 0 and buscfg.mosi_io_num >= 0 and buscfg.sclk_io_num >= 0:

            esp.gpio_pad_select_gpio(self.miso)
            esp.gpio_pad_select_gpio(self.mosi)
            esp.gpio_pad_select_gpio(self.clk)

            esp.gpio_set_direction(self.miso, esp.GPIO_MODE.INPUT)
            esp.gpio_set_pull_mode(self.miso, esp.GPIO.PULLUP_ONLY)
            esp.gpio_set_direction(self.mosi, esp.GPIO_MODE.OUTPUT)
            esp.gpio_set_direction(self.clk, esp.GPIO_MODE.OUTPUT)

            ret = esp.spi_bus_initialize(self.spihost, buscfg, 1)
            if ret != 0:
                raise RuntimeError("Failed initializing SPI bus")
  
    
    def init(self, baudrate=None, phase=None, polarity=None, cs=-1):
        
        if baudrate:
            self.baudrate = baudrate
            
        if phase:
            self.phase = phase
            
        if polarity:
            self.polarity = polarity
            
        if cs > -1:
            self.cs=cs

        if self.spi:
            # Remove device (to change device configuration)
            esp.spi_bus_remove_device(self.spi)
            self.spi = None
            
            
        devcfg_flags = esp.SPI_DEVICE.NO_DUMMY
        if self.half_duplex : devcfg_flags |= esp.SPI_DEVICE.HALFDUPLEX
        
        devcfgSD = esp.spi_device_interface_config_t({
            "clock_speed_hz": self.baudrate,
            "mode": self.phase | self.polarity << 1,
            "spics_io_num": self.cs,
            "queue_size": 2,
            "flags": devcfg_flags,
            "duty_cycle_pos": 128,
        })

        if cs > -1 :
            esp.gpio_pad_select_gpio(self.cs)

        # Attach device to the SPI bus
        ptr_to_spi = esp.C_Pointer()
        ret = esp.spi_bus_add_device(self.spihost, devcfgSD, ptr_to_spi)
        if ret != 0:
            raise RuntimeError("Failed adding SPI device")
            
        self.spi = ptr_to_spi.ptr_val
        self.trans_result_ptr = esp.C_Pointer()
    
    
    def write(self, data):
        if not isinstance(data,bytearray) : data = bytearray(data)
        trans = esp.spi_transaction_t({
            "length": len(data) * 8, 
            "tx_buffer": data,
            "rx_buffer": None,
            "user": None,
        })
        esp.spi_device_queue_trans(self.spi, trans, -1)
        esp.spi_device_get_trans_result(self.spi, self.trans_result_ptr , -1)


    def read(self, length:int, write=None):
        if write: write = bytearray([write])
        buf=bytearray(length)
        trans = esp.spi_transaction_t({
            "length": length * 8,
            "tx_buffer": write,
            "rx_buffer": buf,
            "user": None,
        })
        esp.spi_device_queue_trans(self.spi, trans, -1)
        esp.spi_device_get_trans_result(self.spi, self.trans_result_ptr , -1)
        return bytes(buf)


    def readinto(self, buf:bytearray, write=None):
        if write: write = bytearray([write])
        trans = esp.spi_transaction_t({
            "length": len(buf) * 8,
            "tx_buffer": write,
            "rx_buffer": buf,
            "user": None,
        })
        esp.spi_device_queue_trans(self.spi, trans, -1)
        esp.spi_device_get_trans_result(self.spi, self.trans_result_ptr , -1)


    def write_readinto(self, data, buf:bytearray):
        if not isinstance(data,bytearray) : data = bytearray(data)
        trans = esp.spi_transaction_t({
            "length": len(data) * 8,
            "tx_buffer": data,
            "rx_buffer": buf,
            "user": None,
        })
        esp.spi_device_queue_trans(self.spi, trans, -1)
        esp.spi_device_get_trans_result(self.spi, self.trans_result_ptr , -1)    

              
    def deinit(self):

        if self.spi:
            # Remove device
            esp.spi_bus_remove_device(self.spi)
            self.spi = None

        if self.miso >= 0 and self.mosi >= 0 and self.clk >= 0:
            # Free SPI bus
            esp.spi_bus_free(self.spihost)
            self.spihost = None

