CC        :=	gcc
CC_FLAGS  :=	-m32 -Wall -g
ASM       :=	nasm
ASM_FLAGS :=	-f elf

all: calc.bin

calc.bin: calc.o
	$(CC) $(CC_FLAGS) $^ -o $@

calc.o: calc.s
	$(ASM) $(ASM_FLAGS) $^ -o $@

clean:
	rm -f calc.o calc.bin
