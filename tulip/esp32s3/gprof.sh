cp build/micropython.elf micropython_gprof.elf
xtensa-esp32s3-elf-objcopy -I elf32-xtensa-le --rename-section .flash.text=.text micropython_gprof.elf
xtensa-esp32s3-elf-gprof micropython_gprof.elf gmon.out

