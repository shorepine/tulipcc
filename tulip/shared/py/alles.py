# This is a wrapper for running Alles in AMY on Tulip only. The trifecta!
import struct, os, time, tulip, amy

amy.insert_time = None
mesh_flag = 0

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

def mesh(local_ip=None):
    global mesh_flag
    if(tulip.ip() is None):
        print("Need to be on wifi. Use tulip.wifi('ssid', 'password').")
        return
    amy.send(latency_ms=1000)
    # Explicitly send insert_time arg when using Alles.
    amy.insert_time = tulip.ticks_ms 
    mesh_flag = 1
    if(local_ip is not None):
        tulip.multicast_start(local_ip)
    else:
        tulip.multicast_start("")









