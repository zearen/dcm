FLAGS= -g --std=c++0x -fPIC
LIBS= -Llib -lDcmType
OBJ= Callback.o Plugin.o Interpretter.o ExecParser.o

lib/dcm.so: $(OBJ)  lib/libDcmType.a
	g++ $(FLAGS) -shared -Wl,-soname,libdcm.so -o lib/libdcm.so $(OBJ) $(LIBS)

Callback.o: Callback.cpp Callback.h Interpretter.h Plugin.h
	g++ $(FLAGS) -o Callback.o -c Callback.cpp

Plugin.o: Plugin.cpp Plugin.h Callback.h
	g++ $(FLAGS) -o Plugin.o -c Plugin.cpp

Interpretter.o: Interpretter.cpp Interpretter.h Plugin.h Callback.h
	g++ $(FLAGS) -o Interpretter.o -c Interpretter.cpp

ExecParser.o: Interpretter.h Callback.h
	g++ $(FLAGS) -o ExecParser.o -c ExecParser.cpp

lib/libDcmType.a: force_look
	cd DcmType; make

force_look:
	true

.PHONY: clean
clean:
	cd DcmType; make clean
	rm *.o
	rm lib/*
