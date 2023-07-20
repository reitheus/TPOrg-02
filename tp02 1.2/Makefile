all: mainlru.o cpulru.o generator.o instruction.o memory.o mmu.o lista.o
	@gcc mainlru.o cpulru.o generator.o instruction.o memory.o mmu.o lista.o -o exe -lm
	@rm -r mainlru.o cpulru.o generator.o instruction.o memory.o mmu.o lista.o
mainlru.o: mainlru.c
	@gcc mainlru.c -c -Wall
cpulru.o: cpulru.c
	@gcc cpulru.c -c -Wall
generator.o: generator.c
	@gcc generator.c -c -Wall
instruction.o: instruction.c
	@gcc instruction.c -c -Wall
memory.o: memory.c
	@gcc memory.c -c -Wall
mmu.o: mmu.c
	@gcc mmu.c -c -Wall
lista.o: lista.c
	@gcc lista.c -c -Wall
run:
	@./exe
