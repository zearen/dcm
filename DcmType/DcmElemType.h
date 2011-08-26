/*
    Zachary Weaver: zaw6@pitt.edu
    17/02/2011
    DcmElemType.h
*/

#ifndef _DCM_TYPE
#include "DcmType.h"
#endif

// Elementary Types
#define NONE    0
#define INT     1
#define FLOAT   2
#define CHAR    3
#define BOOL    4
#define STRING  5
#define SYMBOL  6
//#define ALIAS   6
#define EXTEN   8

class DcmElem : public DcmType {
};

class DcmNone : public DcmElem {
    TypeVal type();
    string repr();
};

const DcmNone *dcmNone = new DcmNone;

class DcmInt : public DcmElem {
    public:
        int val;
        DcmInt(DcmInt& toCopy);
        DcmInt(int newVal);
        TypeVal type();
        string repr();
};

class DcmFloat : public DcmElem {
    public:
        double val;
        DcmFloat(DcmFloat& toCopy);
        DcmFloat(double newVal);
        TypeVal type();
        string repr();
};

class DcmChar : public DcmElem {
    public:
        char val;
        DcmChar(DcmChar& toCopy);
        DcmChar(char newVal);
        TypeVal type();
        string repr();
};

class DcmBool : public DcmElem {
    public:
        bool val;
        DcmBool(DcmChar& toCopy);
        DcmBool(bool newVal);
        TypeVal type();
        string repr();
};

class DcmString : public DcmElem, public string {
    public:
        TypeVal type();
        string repr();
};

class DcmSymbol : public DcmElem {
    private:
       string symbol;
    public:
        DcmSymbol(DcmSymbol& toCopy);
        DcmSymbol(string name);
        string get();
        TypeVal type();
        string repr();
};

/*
class DcmAlias : public DcmElem {
    private:
        DcmType *ptr;
    public:
        DcmAlias(DcmAlias& toCopy);
        DcmAlias(DcmType *ref);
        ~DcmAlias();
        DcmType operator*();
        TypeVal type();
        string repr();
};
*/


// Inherit this for custom primitives
class DcmExtension : public DcmType {
    private:
        TypeVal typeVal;
    protected:
        virtual TypeVal subtype() =0;
    public:
        DcmExtension();
        TypeVal type();  // Do not override!
};
