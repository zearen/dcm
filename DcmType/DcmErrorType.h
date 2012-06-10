/*

*/

#ifndef _DCM_ERROR_TYPE_H
#define _DCM_ERROR_TYPE_H
#include "DcmType.h"

// Error Types
#define UNKNOWN_E       0
#define TYPE_E          1
#define OUT_OF_BOUNDS_E 2
#define STACK_EMPTY_E   3
// Bit 3 set in a custom error means runtime custom
#define CUSTOM_E        8


namespace Dcm {
class DcmError : public DcmType {
    public:
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal type();
        static TypeVal typeVal();
        std::string repr();
};

class DcmTypeError : public DcmError {
    protected:
		std::vector<TypeVal> exp;
        TypeVal got;
    public:
        DcmTypeError(DcmTypeError& toCopy);
        DcmTypeError(std::vector<TypeVal> expected, TypeVal recieved);
        bool equals(DcmType& dcm);
        DcmType *copy();
        std::vector<TypeVal> expected();
        TypeVal received();
        TypeVal type();
        static TypeVal typeVal();
        std::string repr();
};

class DcmBoundsError : public DcmError {
    protected:
        int top, giv;
    public:
        DcmBoundsError(DcmBoundsError& toCopy);
        DcmBoundsError(int topBound, int given);
        bool equals(DcmType& dcm);
        DcmType *copy();
        int topBound();
        int given();
        TypeVal type();
        static TypeVal typeVal();
        std::string repr();
};

class DcmStackError : public DcmError {
    protected:
        DcmSymbol* dcmStack;
        TypeVal senderType;
    public:
        DcmStackError(DcmStackError& toCopy);
        DcmStackError(DcmSymbol* sym, TypeVal sentFrom);
        ~DcmStackError();
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal sender();
        DcmSymbol *getSymbol();
        TypeVal type();
        static TypeVal typeVal();
        std::string repr();
}; 

// Inherit this for custom errors
class DcmCustomError : public DcmError {
    protected:
		std::string msg;
        virtual TypeVal subtype();
    public:
        DcmCustomError(DcmCustomError& toCopy);
        DcmCustomError(std::string& message);
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal type();  // Do not override!
        static TypeVal typeVal();
        std::string repr();
};
}
#endif
