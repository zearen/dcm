LIBS= -Llib -lDcmType

lib/libDcmType.a: force_look
	cd DcmType; make

force_look:
	true
