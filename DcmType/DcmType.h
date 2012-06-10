/*
    Zachary Weaver
    26/07/2011
    DcmType.h
*/


#ifndef _DCM_TYPE_H
#define _DCM_TYPE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <stack>

//using namespace std;

/*
Type bits   Subtype bits    Subtype continuation bit
___________ _______________ ___
7   6   5   4   3   2   1   0
*/

// Masks
#define TYPE_MASK 0xE0

// Primitive Types       // dec .h .cpp
#define ELEM        0x00 //  _/ _/ _/  
#define PRIMFUN     0x20 //  _/ _/ _/  
#define ARRAY       0x40 //  _/ _/ _/  
#define EXEC        0x60 //  _/ _/ _/  
#define NAMESPACE   0x80 //  _/ _/ _/  
#define CLASS       0xC0 //  _/ _/ _/  
#define FORK        0xA0 //            
#define ERROR       0xE0 //  _/ _/ _/  

namespace Dcm {

typedef unsigned char* TypeVal;

class DcmType;

typedef std::stack<DcmType*> DcmStack;

class Namespace : public std::unordered_map<std::string, DcmStack> {
    public:
        virtual char id();

        virtual DcmType* restore();
        
        virtual ~Namespace(){}
};

typedef std::stack<Namespace*> Scope;

class DcmType {
    protected:
        unsigned short refs;
        virtual bool equals(DcmType& dcm) =0;
    public:
        DcmType();
        DcmType(DcmType &toCopy);
        virtual ~DcmType(){}
        
        void addRef();
        int refCount();

        virtual DcmType *copy() =0;
        bool isType(TypeVal otherType);
        virtual TypeVal type() =0;
        virtual std::string repr() =0;
        // Returns null if it does not support a class interface
        virtual Namespace* getNamespace();
        // Does a type check, then calls virtual function
        bool operator==(DcmType& dcm);
        bool operator!=(DcmType& dcm);

        
    friend void del(DcmType *dcm);
    friend DcmType *dup(DcmType *dcm);
        
};

// Safely attempt to delete a DcmType
// Reduces refCount and deletes the object if 0
void del(DcmType *dcm);

// Duplicate a DcmType
// Adds one to refCount and can be used in expressions
DcmType *dup(DcmType *dcm);

// Returns a type as a hexadecimal string
std::string typeVal2Str(TypeVal type);

// Checks type equality
bool sameType(TypeVal type1, TypeVal type2);

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
}
//BEGIN Forks
//class DcmFork;

#include "DcmElemType.h"
#include "DcmErrorType.h"
#include "DcmCompositeType.h"
//#include "DcmForkType.h"

#endif
