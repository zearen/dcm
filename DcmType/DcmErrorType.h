#ifndef _DCM_TYPE
#include "DcmType.h"
#endif

// Error Types
#define UNKNOWN_E       0
#define TYPE_E          1
#define OUT_OF_BOUNDS_E 2
#define STACK_EMPTY_E   3
// Bit 3 set in a custom error means runtime custom
#define CUSTOM_E        8

class DcmError : public DcmType {
    public:
        DcmType *copy();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

class DcmTypeError : public DcmError {
    protected:
        TypeVal exp, got;
    public:
        DcmTypeError(DcmTypeError& toCopy);
        DcmTypeError(TypeVal expected, TypeVal received);
        DcmType *copy();
        TypeVal expected();
        TypeVal received();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

class DcmBoundsError : public DcmError {
    protected:
        int top, giv;
    public:
        DcmBoundsError(DcmBoundsError& toCopy);
        DcmBoundsError(int topBound, int given);
        DcmType *copy();
        int topBound();
        int given();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

class DcmStackError : public DcmError {
    protected:
        DcmSymbol* dcmStack;
        TypeVal senderType;
    public:
        DcmStackError(DcmStackError& toCopy);
        DcmStackError(DcmSymbol* sym, TypeVal sentFrom);
        ~DcmStackError();
        DcmType *copy();
        TypeVal sender();
        DcmSymbol *getSymbol();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
}; 

// Inherit this for custom errors
class DcmCustomError : public DcmError {
    protected:
        string msg;
        virtual TypeVal subtype();
    public:
        DcmCustomError(DcmCustomError& toCopy);
        DcmCustomError(string& message);
        DcmType *copy();
        TypeVal type();  // Do not override!
        static TypeVal typeVal();
        string repr();
};
