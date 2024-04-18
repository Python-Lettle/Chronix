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

ENTRY_POINT = 0x7e10
# 编译选项
# -g产生调试信息
# -fno-builtin 不采用C语言的内建函数，解决与内建函数重名问题。
CFLAGS=-m32 -c -fno-builtin -g -std=c99 -Wall -Wextra -I $(INCLUDE_DIR)
LDFLAGS=-m elf_i386 -Tlink.ld

# 内核文件
KERNEL_FILE = $(TARGET_KERNEL_DIR)/kernel.o $(TARGET_KERNEL_DIR)/kernel_lib32.o $(TARGET_KERNEL_DIR)/main.o $(TARGET_KERNEL_DIR)/init.o \
			$(TARGET_KERNEL_DIR)/stdio.o $(TARGET_KERNEL_DIR)/stdlib.o $(TARGET_KERNEL_DIR)/type.o $(TARGET_KERNEL_DIR)/string.o $(TARGET_KERNEL_DIR)/string_asm.o \
			$(TARGET_KERNEL_DIR)/Terminal.o $(TARGET_KERNEL_DIR)/protect.o $(TARGET_KERNEL_DIR)/exception.o \
			$(TARGET_KERNEL_DIR)/interrupt_8259.o $(TARGET_KERNEL_DIR)/keyboard.o

# 运行选项
QEMU_RUN_OPTION = -m 64M

MOUNT_POINT = /mnt/Chronix

all: img
	@echo "系统构建完成！"


boot: $(BOOT_DIR)/boot.asm
	$(AS) $(BOOT_DIR)/boot.asm -o $(TARGET_DIR)/boot.bin -I $(BOOT_DIR)/include
	$(AS) $(BOOT_DIR)/loader.asm -o $(TARGET_DIR)/loader.bin -I $(BOOT_DIR)/include

kernel: $(KERNEL_FILE)
	mkdir -p $(TARGET_KERNEL_DIR)
	$(LD) $(LDFLAGS) -o $(TARGET_DIR)/kernel.elf $^
	objcopy -O binary $(TARGET_DIR)/kernel.elf $(TARGET_DIR)/kernel.bin

img: boot kernel
	dd if=/dev/zero of=$(IMG_NAME) bs=512 count=2880
	dd if=$(TARGET_DIR)/boot.bin of=$(IMG_NAME) conv=notrunc bs=512 count=1
	sudo mount $(IMG_NAME) $(MOUNT_POINT)
	sudo cp -fv $(TARGET_DIR)/loader.bin $(MOUNT_POINT)
	sudo cp -fv $(TARGET_DIR)/kernel.bin $(MOUNT_POINT)
	sudo umount $(MOUNT_POINT)

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
	$(AS) -f elf $(KERNEL_DIR)/kernel.asm -o $(TARGET_KERNEL_DIR)/kernel.o -I $(KERNEL_DIR)

$(TARGET_KERNEL_DIR)/kernel_lib32.o: $(KERNEL_DIR)/kernel_lib32.asm
	$(AS) -f elf $(KERNEL_DIR)/kernel_lib32.asm -o $(TARGET_KERNEL_DIR)/kernel_lib32.o -I $(KERNEL_DIR)

$(TARGET_KERNEL_DIR)/main.o: $(KERNEL_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_KERNEL_DIR)/init.o: $(KERNEL_DIR)/init.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_KERNEL_DIR)/stdio.o: $(KERNEL_DIR)/stdio.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_KERNEL_DIR)/stdlib.o: $(KERNEL_DIR)/stdlib.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_KERNEL_DIR)/type.o: $(KERNEL_DIR)/type.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_KERNEL_DIR)/string.o: $(KERNEL_DIR)/string.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_KERNEL_DIR)/string_asm.o: $(KERNEL_DIR)/string.asm
	$(AS) -f elf $(KERNEL_DIR)/string.asm -o $(TARGET_KERNEL_DIR)/string_asm.o

$(TARGET_KERNEL_DIR)/Terminal.o: $(KERNEL_COMPONENT)/Terminal.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_KERNEL_DIR)/protect.o: $(KERNEL_DIR)/protect.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_KERNEL_DIR)/exception.o: $(KERNEL_DIR)/exception.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_KERNEL_DIR)/interrupt_8259.o: $(KERNEL_DIR)/interrupt_8259.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_KERNEL_DIR)/keyboard.o: $(KERNEL_DIR)/keyboard.c
	$(CC) $(CFLAGS) -o $@ $^