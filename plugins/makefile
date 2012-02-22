FLAGS= -g --std=c++0x -fPIC

io.o: io.h io.cpp ../Interpretter.h
	g++ $(FLAGS) -o io.o -c io.cpp

prelude.o: prelude.h prelude.cpp ../Interpretter.h
	g++ $(FLAGS) -o prelude.o -c prelude.cpp

.PHONY: all
all: io.o prelude.o

.PHONY: clean
clean:
	rm *.o
