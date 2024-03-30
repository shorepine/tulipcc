# This is a wrapper for running Alles in AMY on Tulip only. The trifecta!
import struct, os, time, tulip, amy

def millis():
    # Timestamp to send over to synths for global sync
    # This is a suggestion. I use ticks since boot on Tulip
    return tulip.ticks_ms()

def send(retries=1, **kwargs):
    global mesh_flag
    m = amy.message(**kwargs)
    tulip.alles_send(m, mesh_flag)

def map():
    if mesh_flag:
        tmap = tulip.alles_map()
        # Sort it by clock descending to get client #
        tmap.sort(key=lambda x:x[1], reverse=True)
        return tmap
    print("Need to be on wifi and mesh().")
    return None

def mesh(local_ip=None, local_node=True):
    global mesh_flag
    if(local_node):
        local_node = 1
    else:
        local_node = 0
    if(tulip.ip() is None):
        print("Need to be on wifi. Use tulip.wifi('ssid', 'password').")
        return
    amy.send(latency_ms=1000)
    mesh_flag = 1
    if(local_ip is not None):
        tulip.multicast_start(local_ip, local_node)
    tulip.multicast_start("", local_node)

def local():
    global mesh_flag
    # the default
    mesh_flag = 0
    amy.send(latency_ms=0)








