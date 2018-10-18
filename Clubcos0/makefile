# 변수
Z_TOOLS = z_tools

EDIMG = $(Z_TOOLS)/edimg.exe

QEMU = qemu-system-x86_64w
QEMU_FLAGS = -L . -m 64 -fda floppy.img -boot a -localtime -M pc
QEMU_DEBUG_FLAGS = -gdb tcp:127.0.0.1:1234 -S

# 기본 규칙
all:
	make -C Bootsector
	make -C Kernel
	make -r floppy.img

# 생성 규칙
floppy.img: Bootsector/bin/bootsector.bin Kernel/bin/Clubcos0.sys makefile
	z_tools/edimg imgin:z_tools/fdimg0at.tek \
		wbinimg src:Bootsector/bin/bootsector.bin len:512 from:0 to:0 \
		copy from:Kernel/bin/Clubcos0.sys to:@: \
		imgout:floppy.img

# 커맨드
run:
	make all
	-$(QEMU) $(QEMU_FLAGS)

run_dbg:
	make all
	-$(QEMU) $(QEMU_FLAGS) $(QEMU_DEBUG_FLAGS)

clean:
	make clean -C Bootsector
	make clean -C Kernel

src_only:
	make clean
	-rm floppy.img
