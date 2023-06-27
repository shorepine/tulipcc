# tulip_fs_create.py
# creates the file system mounted on Tulip CC, using littlefs
# takes everything in tulip_home/ex and tulip_home/ex/g and copies it in
# then flashes the partition, which will erase anything you've stored there

from littlefs import lfs
import os
import sys

idf_path = os.environ["IDF_PATH"]  # get value of IDF_PATH from environment
parttool_dir = os.path.join(idf_path, "components", "partition_table")  # parttool.py lives in $IDF_PATH/components/partition_table

sys.path.append(parttool_dir)  # this enables Python to find parttool module
from parttool import *  # import all names inside parttool module


if(not os.getcwd().endswith("esp32s3")):
    print("Run this from the tulipcc/ports/esp32s3 folder only")
    sys.exit()

TULIP_HOME = "../fs"

# Copy over only these extensions 
good_exts = [".txt", ".png", ".py"]
# And these folders
source_folders = ['ex','images', 'test', 'sounds']

cur_dir = os.getcwd()
os.chdir(TULIP_HOME)
folders = source_folders
# Expand the folders
for folder in source_folders:
    folders = folders + [ f.path for f in os.scandir(folder) if f.is_dir() ]

if(len(sys.argv)>1 and sys.argv[1]=='N8R8'):
    TULIP_VFS_SIZE = 0x480000
else:
    TULIP_VFS_SIZE = 0x1832000 

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
    for file in os.listdir(folder):
        file_part, ext = os.path.splitext(file)
        if(ext in good_exts):
            copy_to_lfs(folder+'/'+file, folder+'/'+file)

os.chdir(cur_dir)

print("writing .bin file...")
with open("tulip-lfs.bin","wb") as fh:
    fh.write(cfg.user_context.buffer)
print("... done.")


target = ParttoolTarget()
target.write_partition(PartitionName("vfs"), "tulip-lfs.bin")
os.system('mv tulip-lfs.bin build/')



