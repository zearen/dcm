include ../makefile.in

io.o: io.h io.cpp ../Interpretter.h
	g++ $(FLAGS) -o io.o -c io.cpp

IO_OBJ= io.o
../lib/libplugin_io.a: $(IO_OBJ)
	ar -cq ../lib/libplugin_io.a $(IO_OBJ)

prelude.o: preludeMath.h prelude.h prelude.cpp ../Interpretter.h
	g++ $(FLAGS) -o prelude.o -c prelude.cpp

preludeMath.o: preludeMath.h prelude.h preludeMath.cpp ../Interpretter.h
	g++ $(FLAGS) -o preludeMath.o -c preludeMath.cpp

PRELUDE_OBJ= prelude.o preludeMath.o

../lib/libplugin_prelude.a: $(PRELUDE_OBJ)
	ar -cq ../lib/libplugin_prelude.a $(PRELUDE_OBJ)

.PHONY: all
all: ../lib/libplugin_io.a ../lib/libplugin_prelude.a

.PHONY: clean
clean:
	rm *.o
