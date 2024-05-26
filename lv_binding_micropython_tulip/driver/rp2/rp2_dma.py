import time
import uctypes
import struct
import machine
import sys

if sys.platform!='rp2': raise ImportError('This module can only be meaningfully used on the rp2 platform.')

class DMA:
    DMA_BASE  = const(0x50000000)

    DMA_EN    = const(0x01 << 0)
    HIGH_PRIO = const(0x01 << 1)
    INCR_READ = const(0x01 << 4)
    INCR_WRITE= const(0x01 << 5)
    DREQ_PIO0_RX0 = const(0x04 << 15)
    DREQ_SPI1_TX  = const(0x12 << 15)
    DREQ_PERMANENT= const(0x3F << 15)
    IRQ_QUIET = const(0x01 << 21)
    BUSY      = const(0x01 << 24)

    def __init__( self, channelNumber ):
        offset = channelNumber * 0x40
        self.CHx_READ_ADDR     = DMA.DMA_BASE + 0x00 + offset
        self.CHx_WRITE_ADDR    = DMA.DMA_BASE + 0x04 + offset
        self.CHx_TRANS_COUNT   = DMA.DMA_BASE + 0x08 + offset
        self.CHx_CTRL_TRIG     = DMA.DMA_BASE + 0x0C + offset

    def config( self, src_addr, dst_addr, count, src_inc, dst_inc, trig_dreq ):
        machine.mem32[ self.CHx_CTRL_TRIG ]   = 0
        machine.mem32[ self.CHx_READ_ADDR ]   = src_addr
        machine.mem32[ self.CHx_WRITE_ADDR ]  = dst_addr
        machine.mem32[ self.CHx_TRANS_COUNT ] = count
        trig_val = 0
        if( src_inc ):
            trig_val |= DMA.INCR_READ
        if( dst_inc ):
            trig_val |= DMA.INCR_WRITE
        trig_val |= trig_dreq
        machine.mem32[ self.CHx_CTRL_TRIG ]   = trig_val

    def enable( self ):
        machine.mem32[ self.CHx_CTRL_TRIG ] |= DMA.DMA_EN

    def disable( self ):
        machine.mem32[ self.CHx_CTRL_TRIG ] = 0

    def is_busy( self ):
        if( machine.mem32[ self.CHx_CTRL_TRIG ] & DMA.BUSY ):
            return True
        else:
            return False
