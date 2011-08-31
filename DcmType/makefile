FLAGS=--std=c++0x

OBJ=DcmType.o DcmElemType.o DcmErrorType.o DcmCompositeType.o

../lib/libDcmType.a: $(OBJ)
	ar -cq ../lib/libDcmType.a $(OBJ)

DcmType.o: DcmType.h DcmType.cpp
	g++ $(FLAGS) -c DcmType.cpp

DcmElemType.o: DcmType.h DcmElemType.h DcmElemType.cpp
	g++ $(FLAGS) -c DcmElemType.cpp

DcmErrorType.o: DcmType.h DcmErrorType.h DcmErrorType.cpp
	g++ $(FLAGS) -c DcmErrorType.cpp

DcmCompositeType.o: DcmType.h DcmCompositeType.h DcmCompositeType.cpp
	g++ $(FLAGS) -c DcmCompositeType.cpp
