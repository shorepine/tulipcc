# fs_create.py

from littlefs import lfs
import os
import sys

if(len(sys.argv)<2):
    print("Usage: python fs_create.py (amyboard,tulip) [flash]")

distro = sys.argv[1]
if(distro=='tulip'):
    os.chdir('esp32s3')
else:
    os.chdir('amyboard')

idf_path = os.environ["IDF_PATH"]  # get value of IDF_PATH from environment
parttool_dir = os.path.join(idf_path, "components", "partition_table")  # parttool.py lives in $IDF_PATH/components/partition_table

sys.path.append(parttool_dir)  # this enables Python to find parttool module
from parttool import *  # import all names inside parttool module
import gen_esp32part as gen

if(not os.path.exists('build/flash_args')):
    print("Run this after a successful build only")
    sys.exit()

SYSTEM_HOME = "../fs/%s" % (distro)

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

print("writing VFS .bin file...")
with open("build/%s-vfs.bin" % (distro),"wb") as fh:
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
with open("build/%s-sys.bin" % (distro),"wb") as fh:
    fh.write(cfg.user_context.buffer)
print("... done.")


# Update the flash_args file to have the sys and user partitions
flash_args = open('build/flash_args','r').read().split('\n')[:-1]
flash_args.append('%s %s-sys.bin' % (hex(sys_partition.offset), distro))
flash_args.append('%s %s-vfs.bin' % (hex(vfs_partition.offset), distro))
new_flash_args = open('build/flash_args_%s' % (distro),'w')
for f in flash_args:
    new_flash_args.write('%s\n' % (f))
new_flash_args.close()
os.chdir('build')
os.system('esptool.py --chip esp32s3 merge_bin -o %s.bin @flash_args_%s' % (distro, distro))
os.chdir('..')

# I don't love this but it works
# i wonder if i can get CMake to pass along MICROPY_BOARD to this program in a shell instead
MICROPY_BOARD = subprocess.check_output("grep MICROPY_BOARD build/CMakeCache.txt | cut -d '=' -f2 | awk '{print $1}'",shell=True)[:-1].decode('ascii')
os.system("mkdir -p dist")
os.system("cp build/%s.bin dist/%s-full-%s.bin" % (distro, distro, MICROPY_BOARD))
os.system("cp build/micropython.bin dist/%s-firmware-%s.bin" % (distro, MICROPY_BOARD))
os.system("cp build/%s-sys.bin dist/%s-sys.bin" %(distro, distro))

# Optionally do the flash of the whole image
if(len(sys.argv)>2):
    if(sys.argv[2]== 'flash'):
        print("Writing full image")
        os.system("esptool.py write_flash 0x0 dist/%s-full-%s.bin" % (distro, MICROPY_BOARD))

os.chdir('..')

