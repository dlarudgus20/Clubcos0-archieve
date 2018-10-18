##################################
# Kernel makefile
##################################

# 변수
C_SOURCES = $(wildcard *.c)
C_OBJECTS = $(patsubst %.c, tmp/%.o, $(C_SOURCES))

ASM_OBJECT = tmp/AssemblyFunc.o

GCC_OPT = -O3 -mno-sse
NASM_OPT =

# 기본 규칙
all: Dependency.dep
	##################################
	# Kernel makefile
	##################################

	make -r bin/Clubcos0.sys

# 생성 규칙
bin/Clubcos0.sys: tmp/AsmKernel.bin tmp/C_Kernel.bin makefile
	cat tmp/AsmKernel.bin tmp/C_Kernel.bin > bin/Clubcos0.sys

tmp/AsmKernel.bin: AsmKernel.asm ../Bootsector/init.inc makefile
	nasm -f bin -o tmp/AsmKernel.bin -l tmp/AsmKernel.lst AsmKernel.asm

tmp/AssemblyFunc.o: AssemblyFunc.asm makefile
	nasm -f elf $(NASM_OPT) -o tmp/AssemblyFunc.o -l tmp/AssemblyFunc.lst AssemblyFunc.asm

tmp/C_Kernel.bin: $(ASM_OBJECT) $(C_OBJECTS) makefile
	x86_64-pc-linux-ld -melf_i386 -T elf_i386.x -nostdlib -o tmp/C_Kernel.exe -Ttext 0x500000 -e KernelStartup \
		-Map tmp/C_Kernel.map \
		$(ASM_OBJECT) $(C_OBJECTS)
	x86_64-pc-linux-objcopy -R .note -R .comment -R .absolut -S -O binary tmp/C_Kernel.exe tmp/C_Kernel.bin
	x86_64-pc-linux-nm --line-numbers tmp/C_Kernel.exe | sort > tmp/C_Kernel_nm.txt
	ndisasm -b 32 tmp/C_Kernel.bin > tmp/C_Kernel_disasm.txt

tmp/Dependency.dep: $(C_SOURCES) makefile
	x86_64-pc-linux-gcc -MM $(C_SOURCES) > tmp/Dependency.dep

# 일반 규칙
tmp/%.o: %.c makefile
	x86_64-pc-linux-gcc -m32 -nostdinc -ffreestanding $(GCC_OPT) -o $@ -c $<
	x86_64-pc-linux-objdump -M intel -D $@ > tmp/$*_dump.txt

# 커맨드
clean:
	-rm tmp/*
	-rm bin/*

ifeq (tmp/Dependency.dep, $(wildcard tmp/Dependency.dep))
include tmp/Dependency.dep
endif
