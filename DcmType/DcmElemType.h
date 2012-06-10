/*
    Zachary Weaver: zaw6@pitt.edu
    17/02/2011
    DcmElemType.h
*/

#ifndef _DCM_ELEM_TYPE
#define _DCM_ELEM_TYPE
#include "DcmType.h"

// Elementary Types
#define DCM_NONE    0
#define DCM_INT     1
#define DCM_FLOAT   2
#define DCM_CHAR    3
#define DCM_BOOL    4
#define DCM_STRING  5
#define DCM_SYMBOL  6
#define DCM_EXTEN   8

namespace Dcm {
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
        std::string repr();
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
        std::string repr();
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
        std::string repr();
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
        std::string repr();
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
        std::string repr();
};

class DcmString : public DcmElem, public std::string {
    public:
        DcmString();
        DcmString(std::string toCopy);
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal type();
        static TypeVal typeVal();
        std::string repr();
};

class DcmSymbol : public DcmElem {
    private:
		std::string symbol;
    public:
        DcmSymbol(DcmSymbol& toCopy);
        DcmSymbol(std::string name);
        bool equals(DcmType& dcm);
        DcmType *copy();
        std::string get();
        TypeVal type();
        static TypeVal typeVal();
        std::string repr();
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
}
#endif
