# T U L I P ~ W O R L D
# a Tulip only chat room and file transfer service

from world_matrix import *
import tulip

def put_message(m):
    row = "     " + nice_time(m["age_s"]) + ": " + m["body"]
    print(row[:100])
    if(len(row) > 100):
        # word wrap
        for i in range(int(len(row)/100)- 1):
            print("               " + row[(i+1)*100:(i+2)*100])


def world_ui():
    tulip.gpu_reset()
    print(tulip.Colors.INVERSE + "Welcome to T U L I P ~ W O R L D")
    print(tulip.Colors.DEFAULT)
    tulip.roundrect(20,20,950,500,20,145)
    tulip.roundrect(20,520,950,80,20,145)

# sets up the UI for world
def world():
    if(tulip.ip() is None):
        print("need wifi.")
        return

    world_ui() # set up the world UI

    # Get the initial set of n last messages
    m = check()
    for i in m:
        put_message(i)





