# 指定编译器和汇编器
CC = gcc
AS = nasm
QEMU = qemu-system-i386

# 目录结构
BOOT_DIR = bootloader
KERNEL_DIR = kernel
TARGET_DIR = target

INCLUDE_DIR = include

IMG_NAME = Chronix.img

ENTRY_POINT = 0x2000
# 编译选项
# -g产生调试信息
# -fno-builtin 不采用C语言的内建函数，解决与内建函数重名问题。
CFLAGS=-m32 -c -fno-builtin -g -std=c99 -Wall -Wextra -I $(INCLUDE_DIR)
LDFLAGS=-m elf_i386 -Tlink.ld

# 内核文件
KERNEL_FILE = $(TARGET_DIR)/kernel.o $(TARGET_DIR)/main.o $(TARGET_DIR)/stdio.o

all: img


boot: $(BOOT_DIR)/boot.asm $(BOOT_DIR)/loader.asm
	$(AS) $(BOOT_DIR)/boot.asm -o $(TARGET_DIR)/boot.bin -I $(BOOT_DIR)/include
	$(AS) $(BOOT_DIR)/loader.asm -o $(TARGET_DIR)/loader.bin -I $(BOOT_DIR)/include

kernel: $(KERNEL_FILE)
	$(LD) $(LDFLAGS) -o $(TARGET_DIR)/kernel.elf $^
	objcopy -O binary $(TARGET_DIR)/kernel.elf $(TARGET_DIR)/kernel.bin

img: boot
	dd if=$(TARGET_DIR)/boot.bin of=$(IMG_NAME) conv=notrunc bs=512 count=1

clean:
	rm $(TARGET_DIR)/* $(IMG_NAME)

run:
	$(QEMU) $(IMG_NAME)
    
debug:
	$(QEMU) -s -S $(IMG_NAME)

# 内核文件编译规则
$(TARGET_DIR)/main.o: $(KERNEL_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $(KERNEL_DIR)/main.c

$(TARGET_DIR)/kernel.o: $(KERNEL_DIR)/kernel.asm
	$(AS) -f elf $(KERNEL_DIR)/kernel.asm -o $(TARGET_DIR)/kernel.o -I bootloader/include

$(TARGET_DIR)/stdio.o: $(KERNEL_DIR)/stdio.c
	$(CC) $(CFLAGS) -o $@ $(KERNEL_DIR)/stdio.c

