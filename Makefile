CC=clang -ffreestanding -nostdinc -nostdlib -target x86_64-elf -fPIC -isystem newlib/newlib/libc/include -isystem include
COPTS=\
	-Irts \
	-DIDRIS_TARGET_OS='"IdrOS"' \
	-DIDRIS_TARGET_TRIPLE='"x86_64-unknown-IdrOS"'

LIBS=-L. -lrts -Lnewlib/newlib -lc -lm -lrts

.s.o:
	$(CC) -g -c -o $@ $(COPTS) $<

.c.o:
	$(CC) -g -c -o $@ $(COPTS) $<

all: kernel.elf

newlib/newlib/libc.a:
	cd newlib/newlib && CC="clang -isystem `pwd`/include -isystem `pwd`/../../include -ffreestanding -nostdinc -nostdlib -target x86_64-elf -fPIC" ./configure --target=x86_64-elf
	make -C newlib/newlib

clean:
	rm -rf *.o rts/*.o lib/*.o *.a

librts.a: $(patsubst rts/%.c,rts/%.o,$(wildcard rts/*.c)) $(patsubst lib/%.s,lib/%.o,$(wildcard lib/*.s)) $(patsubst lib/%.c,lib/%.o,$(wildcard lib/*.c))
	rm -f $@
	ar cq $@ $^
	ranlib $@

main.c: src/*.idr
	idris -i src -S -o main.c src/Main.idr

kernel.elf: main.o librts.a newlib/newlib/libc.a
	$(CC) -g -o $@ main.o $(LIBS)
