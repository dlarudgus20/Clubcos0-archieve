##################################
# Bootsector makefile
##################################

# 기본 규칙
all:
	##################################
	# Bootsector makefile
	##################################

	make -r bin/bootsector.bin

# 생성 규칙
bin/bootsector.bin: bootsector.asm init.inc makefile
	nasm -f bin -o bin/bootsector.bin -l tmp/bootsector.lst bootsector.asm

clean:
	-rm bin/*
	-rm tmp/*
