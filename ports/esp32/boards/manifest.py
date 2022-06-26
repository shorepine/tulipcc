freeze("$(PORT_DIR)/modules")
freeze("$(MPY_DIR)/tools", ("upip.py", "upip_utarfile.py"))
freeze("$(MPY_DIR)/ports/esp8266/modules", "ntptime.py")
freeze("$(MPY_DIR)/drivers/dht", "dht.py")
freeze("$(MPY_DIR)/drivers/onewire")
# We have our own version now
#freeze("$(MPY_LIB_DIR)/python-ecosys/urequests", "urequests.py")

freeze("$(MPY_LIB_DIR)/micropython/upysh", "upysh.py")
freeze("$(MPY_LIB_DIR)/micropython/umqtt.simple", "umqtt/simple.py")
freeze("$(MPY_LIB_DIR)/micropython/umqtt.robust", "umqtt/robust.py")
include("$(MPY_DIR)/extmod/uasyncio/manifest.py")
include("$(MPY_DIR)/extmod/webrepl/manifest.py")
include("$(MPY_DIR)/drivers/neopixel/manifest.py")
