CC=clang -m64 -static -Wl,-Tboot/i386/kernel.lds -Wl,-N -g -fPIC -ffreestanding -nostdinc -nostdlib -target x86_64-elf -fPIC -isystem newlib/newlib/libc/include -isystem include
COPTS=\
	-Irts \
	-DIDRIS_TARGET_OS='"IdrOS"' \
	-DIDRIS_TARGET_TRIPLE='"x86_64-unknown-IdrOS"'
LIBS=-L. -lrts -Lnewlib/newlib -lc -lm -lrts

OBJS= \
	$(patsubst rts/%.c,rts/%.o,$(wildcard rts/*.c)) \
	$(patsubst lib/%.c,lib/%.o,$(wildcard lib/*.c))

.s.o:
	$(CC) -g -c -o $@ $(COPTS) $<

.c.o:
	$(CC) -g -c -o $@ $(COPTS) $<

all: kernel.elf boot-i386

boot-i386:
	$(MAKE) -C boot/i386

newlib/newlib/libc.a:
	cd newlib/newlib && CC="clang -isystem `pwd`/include -isystem `pwd`/../../include -ffreestanding -nostdinc -nostdlib -target x86_64-elf -fPIC" ./configure --target=x86_64-elf
	make -C newlib/newlib

clean:
	rm -rf *.o rts/*.o lib/*.o *.a

librts.a: $(OBJS)
	rm -f $@
	ar cq $@ $^
	ranlib $@

main.c: src/*.idr
	idris -i src -S -o main.c src/Main.idr

start.o: src/Arch/startamd64.s
	$(CC) -g -c -o $@ $(COPTS) $<

kernel.elf: start.o main.o librts.a newlib/newlib/libc.a
	$(CC) -g -o $@ start.o main.o $(LIBS)
