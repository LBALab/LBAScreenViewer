CPP=g++

all: bmptofla HQRlib.o

bmptofla: bmptofla.cpp
	$(CPP) bmptofla.cpp -DWITHOUT_BORLAND_COMPILER -o bmptofla

HQRlib.o: HQRlib.cpp
	$(CPP) -c HQRlib.cpp -DWITHOUT_BORLAND_COMPILER

clean:
	rm bmptofla HQRlib.o
