# 指定编译器和汇编器
CC = gcc
AS = nasm
QEMU = qemu-system-i386
BOCHS = bochs

# 目录结构
BOOT_DIR = bootloader
KERNEL_DIR = kernel
KERNEL_COMPONENT = $(KERNEL_DIR)/components
TARGET_DIR = target
TARGET_KERNEL_DIR = $(TARGET_DIR)/kernel

INCLUDE_DIR = include

IMG_NAME = Chronix.img

ENTRY_POINT = 0x2000
# 编译选项
# -g产生调试信息
# -fno-builtin 不采用C语言的内建函数，解决与内建函数重名问题。
CFLAGS=-m32 -c -fno-builtin -g -std=c99 -Wall -Wextra -I $(INCLUDE_DIR)
LDFLAGS=-m elf_i386 -Tlink.ld

# 内核文件
KERNEL_FILE = $(TARGET_KERNEL_DIR)/kernel.o $(TARGET_KERNEL_DIR)/kernel_lib32.o $(TARGET_KERNEL_DIR)/main.o \
			$(TARGET_KERNEL_DIR)/stdio.o $(TARGET_KERNEL_DIR)/stdlib.o $(TARGET_KERNEL_DIR)/type.o $(TARGET_KERNEL_DIR)/string.o \
			$(TARGET_KERNEL_DIR)/Terminal.o

# 运行选项
QEMU_RUN_OPTION = -m 64M

all: img


boot: $(BOOT_DIR)/boot.asm $(BOOT_DIR)/loader.asm
	$(AS) $(BOOT_DIR)/boot.asm -o $(TARGET_DIR)/boot.bin -I $(BOOT_DIR)/include
	$(AS) $(BOOT_DIR)/loader.asm -o $(TARGET_DIR)/loader.bin -I $(BOOT_DIR)/include

kernel: $(KERNEL_FILE)
	mkdir -p $(TARGET_KERNEL_DIR)
	$(LD) $(LDFLAGS) -o $(TARGET_DIR)/kernel.elf $^
	objcopy -O binary $(TARGET_DIR)/kernel.elf $(TARGET_DIR)/kernel.bin

img: boot
	dd if=/dev/zero of=$(IMG_NAME) bs=512 count=2880
	dd if=$(TARGET_DIR)/boot.bin of=$(IMG_NAME) conv=notrunc bs=512 count=1

clean:
	rm -rf $(TARGET_DIR)/*
	rm $(IMG_NAME)

run:
	$(QEMU) $(QEMU_RUN_OPTION) $(IMG_NAME)
    
debug:
	$(QEMU) $(QEMU_RUN_OPTION) $(IMG_NAME) -s -S
# $(BOCHS) -q

# 内核文件编译规则
$(TARGET_KERNEL_DIR)/kernel.o: $(KERNEL_DIR)/kernel.asm
	mkdir -p $(TARGET_KERNEL_DIR)
	$(AS) -f elf $(KERNEL_DIR)/kernel.asm -o $(TARGET_KERNEL_DIR)/kernel.o -I bootloader/include

$(TARGET_KERNEL_DIR)/kernel_lib32.o: $(KERNEL_DIR)/kernel_lib32.asm
	$(AS) -f elf $(KERNEL_DIR)/kernel_lib32.asm -o $(TARGET_KERNEL_DIR)/kernel_lib32.o -I bootloader/include

$(TARGET_KERNEL_DIR)/main.o: $(KERNEL_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $(KERNEL_DIR)/main.c

$(TARGET_KERNEL_DIR)/stdio.o: $(KERNEL_DIR)/stdio.c
	$(CC) $(CFLAGS) -o $@ $(KERNEL_DIR)/stdio.c

$(TARGET_KERNEL_DIR)/stdlib.o: $(KERNEL_DIR)/stdlib.c
	$(CC) $(CFLAGS) -o $@ $(KERNEL_DIR)/stdlib.c

$(TARGET_KERNEL_DIR)/type.o: $(KERNEL_DIR)/type.c
	$(CC) $(CFLAGS) -o $@ $(KERNEL_DIR)/type.c

$(TARGET_KERNEL_DIR)/string.o: $(KERNEL_DIR)/string.c
	$(CC) $(CFLAGS) -o $@ $(KERNEL_DIR)/string.c

$(TARGET_KERNEL_DIR)/Terminal.o: $(KERNEL_COMPONENT)/Terminal.c
	$(CC) $(CFLAGS) -o $@ $(KERNEL_COMPONENT)/Terminal.c
