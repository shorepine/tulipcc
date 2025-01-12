# Run demos
run('voices'), run('juno6'), run('drums')
run('bunny_bounce') or run('planet_boing') or run('parallax')
run('xanadu')
run('wordpad'), run('buttons')

# getting started
tulip.wifi('ssid', 'password')
tulip.upgrade()   # super vital
run('calibrate')  # optional
edit("boot.py")  # start wifi here, print readme


# Connect to tulip BBS
world.prompt_username()
world.download(filename)
world.upload(filename)
world.messges()

# live USB connection to laptop
pip install mpremote # on laptop side
mpremote # connect to the REPL on the tulipcc
mpremote resume fs cp local_file.py :tulip_file.py
mpremote resume fs cp :tulip_file.py local_file.py
mpremote resume edit file.py
mpremote resume mount

# live wifi connection to laptop
file_server.py # on laptop

# local file sys navigation
pwd
ls
cat('boot.py')
mkdir('directory')
cd('directory')

# add wifi command to boot.py so it auto connects on power up
# which is optional :)
edit("boot.py")

usage = tulip.cpu() # CPU usage, 1st call is not valid
tulip.cpu(1) # show more detail in a connected UART

ms = tulip.ticks_ms() # milliseconds since boot, aka Arduino millis() 

board = tulip.board() # Returns the board type, e.g. "TDECK", "N16R8" et

# Running your own programs
execfile("hello.py")

import my_library
my_library.do_something()

# Look into tulip "packages" for apps with multimedia resources like png and wav

# Fancy stuff


# M5 8encoder
import m5_8encoder.py as enc
enc.read_all_counters()
run('m5_8encoder') returns i2c driver install error



