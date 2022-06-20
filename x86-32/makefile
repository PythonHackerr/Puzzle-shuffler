CC=gcc
ASMBIN=nasm

all : asm cc link
asm :
	$(ASMBIN) -g -f elf32 shuffle.asm
cc :
	$(CC)  -m32 -std=c99 -g -c main.c
link :
	$(CC)  -m32 -o main main.o shuffle.o -lm

clean :
	rm *.o
