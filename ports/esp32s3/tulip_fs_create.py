# tulip_fs_create.py
# creates the file system mounted on Tulip CC, using littlefs
# takes everything in tulip_home/ex and tulip_home/ex/g and copies it in
# then flashes the partition, which will erase anything you've stored there

from littlefs import lfs
import os
import sys

if(not os.getcwd().endswith("esp32s3")):
    print("Run this from the tulipcc/ports/esp32s3 folder only")
    sys.exit()

# Copy over only these extensions 
good_exts = [".txt", ".png", ".py"]
# And these folders
folders = ['ex','ex/g']

TULIP_HOME="../../tulip_home"
if(len(sys.argv)>1 and sys.argv[1]=='N8R8'):
    TULIP_VFS_SIZE = 0x480000
else:
    TULIP_VFS_SIZE = 0x919000 

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

for folder in folders:
    lfs.mkdir(fs,folder)
    for file in os.listdir(TULIP_HOME+'/'+folder):
        file_part, ext = os.path.splitext(file)
        if(ext in good_exts):
            copy_to_lfs(TULIP_HOME+'/'+folder+'/'+file, folder+'/'+file)

print("writing .bin file...")
with open("tulip-lfs.bin","wb") as fh:
    fh.write(cfg.user_context.buffer)
print("... done.")

# Now overwrite the user flash partition
os.system('parttool.py write_partition --partition-name=vfs --input=tulip-lfs.bin')
os.system('mv tulip-lfs.bin build/')



