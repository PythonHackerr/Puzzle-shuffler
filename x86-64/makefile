CC=gcc
ASMBIN=nasm

all : asm cc link

asm :
	$(ASMBIN) -g -f elf64 shuffle.asm
cc :
	$(CC)  -m64 -g -c main.c
link :
	$(CC)  -m64 -o main main.o shuffle.o -lm
clean :
	rm *.o
