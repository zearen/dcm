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
#define EXTEN   8

// This categorizes atomic types
class DcmElem : public DcmType {
};

// Types that act like numbers
class DcmNum : public DcmElem {
    // Eventually we'll add simple arithmetic here.
};

class DcmNone : public DcmElem {
    public:
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

extern DcmNone *dcmNone;

class DcmInt : public DcmNum {
    public:
        int val;
        DcmInt(DcmInt& toCopy);
        DcmInt(int newVal);
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

class DcmFloat : public DcmNum {
    public:
        double val;
        DcmFloat(DcmFloat& toCopy);
        DcmFloat(double newVal);
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

class DcmChar : public DcmNum {
    public:
        char val;
        DcmChar(DcmChar& toCopy);
        DcmChar(char newVal);
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

class DcmBool : public DcmElem {
    public:
        bool val;
        DcmBool(DcmBool& toCopy);
        DcmBool(bool newVal);
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

class DcmString : public DcmElem, public string {
    public:
        DcmString();
        DcmString(string& toCopy);
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

class DcmSymbol : public DcmElem {
    private:
       string symbol;
    public:
        DcmSymbol(DcmSymbol& toCopy);
        DcmSymbol(string name);
        bool equals(DcmType& dcm);
        DcmType *copy();
        string get();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

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
