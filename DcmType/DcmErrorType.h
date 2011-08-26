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
    protected:
        static const string msg;
    public:
        DcmError();
        DcmError(DcmError& toCopy);
        TypeVal type();
        string repr();
};

class DcmTypeError : public DcmError {
    protected:
        static const string msg;
        TypeVal exp, got;
    public:
        DcmTypeError(DcmTypeError& toCopy);
        DcmTypeError(TypeVal expected, TypeVal received);
        TypeVal type();
        string repr();
        TypeVal expected();
        TypeVal received();
};

class DcmBoundsError : public DcmError {
    protected:
        static const string msg;
        int top, giv;
    public:
        DcmBoundsError(DcmBoundsError& toCopy);
        DcmBoundsError(int topBound, int given);
        TypeVal type();
        string repr();
        int topBound();
        int given();
};

class DcmStackError : public DcmError {
    protected:
        static const string msg;
        DcmSymbol dcmStack;
        TypeVal sender;
    public:
        DcmStackError(DcmStackError& toCopy);
        DcmStackError(DcmSymbol sym, TypeVal sentFrom);
        TypeVal type();
        string repr();
        TypeVal senderType();
        DcmSymbol getSymbol();
}; 

// Inherit this for custom errors
class DcmCustomError : public DcmError {
    protected:
        static const string msg;
        string customMsg;
    public:
        DcmCustomError(DcmCustomError& toCopy);
        DcmCustomError(string& message);
        TypeVal type();  // Do not override!
        virtual TypeVal subtype();
        string repr();
};
