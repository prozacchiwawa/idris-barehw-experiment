OBJS=boot-i386.o cpuid.o extfun.o longmode.o con32.o multiboot.o memfns.o sse.o
CC=clang -m32 -ffreestanding -nostdinc -nostdlib -target i386-elf -fPIC

all: boot-i386 testkernel.elf

clean:
	rm -f boot-i386 *.o testkernel.elf

.s.o:
	$(CC) -c -g -o $@ $<

.c.o:
	$(CC) -c -g -xc++ -std=c++11 -o $@ $<

testkernel.elf: testkernel.s
	clang -m64 -static -Wl,-Tkernel.lds -Wl,-N -g -nostdlib -target x86_64-elf -fPIC -o testkernel.elf testkernel.s

boot-i386: start.o $(OBJS) end.o
	$(CC) -m32 -Wl,-e,_start -g start.o $(OBJS) end.o -o $@

