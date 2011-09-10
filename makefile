FLAGS= -g --std=c++0x -fPIC
LIBS= -Llib -lDcmType

lib/dcm.so: Callback.o Plugin.o Interpretter.o lib/libDcmType.a
	g++ $(FLAGS) -shared -Wl,-soname,libdcm.so -o lib/libdcm.so Callback.o Plugin.o Interpretter.o $(LIBS)

Callback.o: Callback.cpp Callback.h Interpretter.h Plugin.h
	g++ $(FLAGS) -o Callback.o -c Callback.cpp

Plugin.o: Plugin.cpp Plugin.h Callback.h
	g++ $(FLAGS) -o Plugin.o -c Plugin.cpp

Interpretter.o: Interpretter.cpp Interpretter.h Plugin.h Callback.h
	g++ $(FLAGS) -o Interpretter.o -c Interpretter.cpp

lib/libDcmType.a: force_look
	cd DcmType; make

force_look:
	true

.PHONY: clean
clean:
	cd DcmType; make clean
	rm *.o
	rm lib/*
