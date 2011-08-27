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
        TypeVal type();
        string repr();
};

class DcmTypeError : public DcmError {
    protected:
        TypeVal exp, got;
    public:
        DcmTypeError(DcmTypeError& toCopy);
        DcmTypeError(TypeVal expected, TypeVal received);
        TypeVal expected();
        TypeVal received();
        TypeVal type();
        string repr();
};

class DcmBoundsError : public DcmError {
    protected:
        int top, giv;
    public:
        DcmBoundsError(DcmBoundsError& toCopy);
        DcmBoundsError(int topBound, int given);
        int topBound();
        int given();
        TypeVal type();
        string repr();
};

class DcmStackError : public DcmError {
    protected:
        DcmSymbol dcmStack;
        TypeVal sender;
    public:
        DcmStackError(DcmStackError& toCopy);
        DcmStackError(DcmSymbol sym, TypeVal sentFrom);
        TypeVal senderType();
        DcmSymbol getSymbol();
        TypeVal type();
        string repr();
}; 

// Inherit this for custom errors
class DcmCustomError : public DcmError {
    protected:
        string customMsg;
    public:
        DcmCustomError(DcmCustomError& toCopy);
        DcmCustomError(string& message);
        TypeVal type();  // Do not override!
        virtual TypeVal subtype();
        string repr();
};
