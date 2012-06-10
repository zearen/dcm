include makefile.in

LIBS= -Llib -lDcmType
OBJ= Callback.o Plugin.o Interpretter.o ExecParser.o

all: dcmi
	true

lib/libdcm.so: $(OBJ) lib/libDcmType.a
	g++ $(FLAGS) -shared -Wl,-soname,libdcm.so -o lib/libdcm.so $(OBJ) $(LIBS)
	sudo cp lib/libdcm.so /usr/lib

lib/libdcm.a: $(OBJ) lib/libDcmType.a
	cp lib/libDcmType.a lib/libdcm.a
	ar -ru lib/libdcm.a $(OBJ)

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

.PHONY: plugins
plugins:
	cd plugins; make all

PLUGINS= -lplugin_io -lplugin_prelude
dcmi: lib/libdcm.a Interpretter.h plugins
	g++ $(FLAGS) -o dcmi dcmi.cpp -Llib -ldcm -lreadline $(PLUGINS)

force_look:
	true

.PHONY: clean
clean:
	cd DcmType; make clean
	cd plugins; make clean
	rm *.o
	rm lib/*
