/*
    Zachary Weaver
    26/07/2011
    DcmType.h
*/


#ifndef _DCM_TYPE
#define _DCM_TYPE

#include <string>
#include <vector>
#include <unordered_map>
#include <stack>

using namespace std;

/*
Type bits   Subtype bits    Subtype continuation bit
___________ _______________ ___
7   6   5   4   3   2   1   0
*/

// Masks
#define TYPE_MASK 0xE0

// Primitive Types       // dec .h .cpp
#define ELEM        0x00 //  _/ _/     
#define PRIMFUN     0x20 //  _/ _/     
#define ARRAY       0x40 //  _/ _/     
#define EXEC        0x60 //  _/ _/     
#define NAMESPACE   0x80 //  _/ _/     
#define CLASS       0xC0 //  _/ _/     
#define FORK        0xA0 //            
#define ERROR       0xE0 //  _/ _/     


typedef unsigned char* TypeVal;

class DcmType {
    private:
        unsigned short refs;
    public:
        DcmType();
        DcmType(DcmType &toCopy);
        bool isType(TypeVal otherType);
        void addRef();
        virtual TypeVal type() =0;
        virtual string repr() =0;
};

typedef stack<DcmType*> DcmStack;
typedef unordered_map<string, DcmStack> Namespace;

//string reprType(TypeVal type);

//BEGIN Error types
class DcmError;
class DcmTypeError;
class DcmBoundsError;
class DcmStackError;
// Inherit this for custom errors
class DcmCustomError;

//BEGIN Elem types
class DcmElem;
class DcmInt;
class DcmFloat;
class DcmChar;
class DcmString;
class DcmBool;
class DcmSymbol;
// Inherit this for custom primitives
class DcmExtension;

//BEGIN Composites
class DcmArray;
class DcmNamespace;
class DcmClass;
class DcmPrimFun;
class DcmExec;

//BEGIN Forks
//class DcmFork;

#include "DcmElemType.h"
#include "DcmErrorType.h"
#include "DcmCompositeType.h"
//#include "DcmForkType.h"
#endif
