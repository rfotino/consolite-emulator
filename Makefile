CFLAGS := -Wall -Wextra -Werror -std=c++11
LIBS := -lX11 -lcairo -pthread

all: emu.o vidmem.o window.o processor.o
	g++ $(CFLAGS) -o emu bin/emu.o bin/vidmem.o bin/window.o \
	bin/processor.o $(LIBS)

emu.o: src/emu.cpp src/vidmem.h src/window.h src/processor.h
	g++ $(CFLAGS) -o bin/emu.o -c src/emu.cpp

vidmem.o: src/vidmem.cpp src/vidmem.h src/defs.h
	g++ $(CFLAGS) -o bin/vidmem.o -c src/vidmem.cpp

window.o: src/window.cpp src/window.h src/vidmem.h src/defs.h
	g++ $(CFLAGS) -o bin/window.o -c src/window.cpp

processor.o: src/processor.cpp src/processor.h src/defs.h
	g++ $(CFLAGS) -o bin/processor.o -c src/processor.cpp

clean:
	rm -f emu bin/*.o src/*~
