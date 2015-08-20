CFLAGS := -Wall -Wextra -Werror -std=c++11
LIBS := -lX11 -lcairo -pthread

all: emu.o vidmem.o window.o processor.o
	g++ $(CFLAGS) -o emu emu.o vidmem.o window.o processor.o $(LIBS)

emu.o: emu.cpp vidmem.h window.h processor.h
	g++ $(CFLAGS) -c emu.cpp

vidmem.o: vidmem.cpp vidmem.h defs.h
	g++ $(CFLAGS) -c vidmem.cpp

window.o: window.cpp window.h vidmem.h defs.h
	g++ $(CFLAGS) -c window.cpp

processor.o: processor.cpp processor.h defs.h
	g++ $(CFLAGS) -c processor.cpp

clean:
	rm -f emu *.o *~
