CPP=g++

all: bmptofla

bmptofla: bmptofla.cpp
	$(CPP) bmptofla.cpp -DWITHOUT_BORLAND_COMPILER -o bmptofla

clean:
	rm bmptofla
