# tulip_fs_create.py
# creates the file system mounted on Tulip CC, using littlefs
from littlefs import lfs
import os
import sys

if(not os.getcwd().endswith("esp32")):
    print("Run this from the ports/esp32 folder only")
    sys.exit()

TULIP_HOME="../../tulip_home"
TULIP_VFS_SIZE = 0x500000
cfg = lfs.LFSConfig(block_size=4096, block_count = TULIP_VFS_SIZE / 4096)
fs = lfs.LFSFilesystem()
lfs.format(fs, cfg)
lfs.mount(fs, cfg)

def copy_to_lfs(source, dest):
    print("Copying %s to %s" % (source, dest))
    source_data = open(source, "rb").read()
    fh = lfs.file_open(fs, dest, "wb")
    lfs.file_write(fs, fh, source_data)
    lfs.file_close(fs, fh)

# Create the ex and ex/g folders...
lfs.mkdir(fs,'ex')
lfs.mkdir(fs,'ex/g')

# Copy everything in ex
for f in os.listdir(TULIP_HOME+'/ex'):
    if(f.endswith('.py')):
        copy_to_lfs(TULIP_HOME+'/ex/%s' % (f), 'ex/%s' % (f))

# Copy everything in ex/g
for f in os.listdir(TULIP_HOME+'/ex/g'):
    if(f.endswith('.png')):
        copy_to_lfs(TULIP_HOME+'/ex/g/%s' % (f), 'ex/g/%s' % (f))

print("writing .bin file...")
with open("tulip-lfs.bin","wb") as fh:
    fh.write(cfg.user_context.buffer)
print("... done.")

# You then run 
os.system('parttool.py write_partition --partition-name=vfs --input=tulip-lfs.bin')




