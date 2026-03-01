# amyboard_fs_create.py

from littlefs import lfs
import os
import sys

idf_path = os.environ["IDF_PATH"]  # get value of IDF_PATH from environment
parttool_dir = os.path.join(idf_path, "components", "partition_table")  # parttool.py lives in $IDF_PATH/components/partition_table

sys.path.append(parttool_dir)  # this enables Python to find parttool module
from parttool import *  # import all names inside parttool module
import gen_esp32part as gen


if(not os.getcwd().endswith("amyboard")):
    print("Run this from the tulipcc/tulip/amyboard folder only")
    sys.exit()

if(not os.path.exists('build/flash_args')):
    print("Run this after a successful build only")
    sys.exit()

SYSTEM_HOME = "../fs/amyboard"

# Copy over only these extensions 
good_exts = [".txt", ".png", ".py", ".json", ".obj", ".wav"]
# And these folders
source_folders = ['app','ex','im']

# Get the partition info from the built partition table
partition_table_file = "build/partition_table/partition-table.bin"
with open(partition_table_file, 'rb') as f:
    partition_table = gen.PartitionTable.from_binary(f.read())
vfs_partition = partition_table.find_by_name('vfs')
sys_partition = partition_table.find_by_name('system')

def copy_to_lfs(source, dest):
    #print("Copying %s to %s" % (source, dest))
    source_data = open(source, "rb").read()
    fh = lfs.file_open(fs, dest, "wb")
    lfs.file_write(fs, fh, source_data)
    lfs.file_close(fs, fh)

# First make an empty VFS for the user filesystem
cfg = lfs.LFSConfig(block_size=4096, block_count = int(vfs_partition.size / 4096),  disk_version=0x00020000)
fs = lfs.LFSFilesystem()
lfs.format(fs, cfg)
lfs.mount(fs, cfg)
copy_to_lfs('boot.py', 'boot.py')
try:
    lfs.mkdir(fs, 'current')
except:
    pass
copy_to_lfs('env.py', 'current/env.py')

print("writing VFS .bin file...")
with open("build/amyboard-vfs.bin","wb") as fh:
    fh.write(cfg.user_context.buffer)
print("... done.")

cur_dir = os.getcwd()
os.chdir(SYSTEM_HOME)
folders = [x[0][2:] for x in os.walk('.')][1:]

cfg = lfs.LFSConfig(block_size=4096, block_count = int(sys_partition.size / 4096),  disk_version=0x00020000)
fs = lfs.LFSFilesystem()
lfs.format(fs, cfg)
lfs.mount(fs, cfg)

for folder in folders:
    lfs.mkdir(fs,folder)
    for file in os.listdir(folder):
        file_part, ext = os.path.splitext(file)
        if(ext in good_exts):
            copy_to_lfs(folder+'/'+file, folder+'/'+file)

os.chdir(cur_dir)

print("writing sys .bin file...")
with open("build/amyboard-sys.bin","wb") as fh:
    fh.write(cfg.user_context.buffer)
print("... done.")


# Update the flash_args file to have the sys and user partitions
flash_args = open('build/flash_args','r').read().split('\n')[:-1]
flash_args.append('%s amyboard-sys.bin' % (hex(sys_partition.offset)))
flash_args.append('%s amyboard-vfs.bin' % (hex(vfs_partition.offset)))
new_flash_args = open('build/flash_args_amyboard','w')
for f in flash_args:
    new_flash_args.write('%s\n' % (f))
new_flash_args.close()
os.chdir('build')
os.system('esptool.py --chip esp32s3 merge_bin -o amyboard.bin @flash_args_amyboard')
os.chdir('..')

os.system("mkdir -p dist")
os.system("cp build/amyboard.bin dist/amyboard-full.bin")
os.system("cp build/micropython.bin dist/amyboard-firmware.bin")
os.system("cp build/amyboard-sys.bin dist/amyboard-sys.bin")

# Optionally do the writes, 
# you can pass nothing (Just creates the files), "full" (writes everything), "sys" (writes the system dir), "vfs" (writes the empty space for /user)
if(len(sys.argv)>1):
    for arg in sys.argv[1:]:
        if(arg == 'full'):
            print("Writing full image")
            os.system("esptool.py write_flash 0x0 dist/amyboard-full.bin")
        if(arg == 'vfs'):
            print("Writing VFS")
            target = ParttoolTarget()
            target.write_partition(PartitionName("vfs"), "build/amyboard-vfs.bin")
        if(arg == 'sys'):
            print("Writing sys")
            target = ParttoolTarget()
            target.write_partition(PartitionName("system"), "build/amyboard-sys.bin")
else:
    print("All done creating images. You can optionally call me with arguments [full, vfs, sys] to flash those directly.")
