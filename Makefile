obj=main.o instruction.o registers.o prefix.o opcode.o modrm.o immediates.o

all: x86 $(obj) 

x86: $(obj) 
	gcc $(obj) -o x86

main.o: main.c 
	gcc -c main.c

instruction.o: instruction.c
	gcc -c instruction.c

registers.o: registers.c
	gcc -c registers.c

prefix.o: prefix.c 
	gcc -c prefix.c

opcode.o: opcode.c 
	gcc -c opcode.c

modrm.o: modrm.c 
	gcc -c modrm.c

immediates.o: immediates.c 
	gcc -c immediates.c

clean:
	rm -f $(obj) x86
	

