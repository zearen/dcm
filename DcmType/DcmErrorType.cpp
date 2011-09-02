/*
    Zachary Weaver
    
    DcmErrorType.cpp
*/

#include "DcmType.h"

// DcmError {
    DcmType *DcmError::copy() {
        return new DcmError(*this);
    }
    
    TypeVal DcmError::type() {
        static unsigned char typeVal[] = {UNKNOWN_E};
        return typeVal;
    }
    
    string DcmError::repr() {
        return "Error: unknown";
    }
// };

// DcmTypeError {
    DcmTypeError::DcmTypeError(DcmTypeError& toCopy) {
        exp = toCopy.exp;
        got = toCopy.got;
    }
    
    DcmTypeError::DcmTypeError(TypeVal expected, TypeVal received) {
        exp = expected;
        got = received;
    }
    
    DcmType *DcmTypeError::copy() {
        return new DcmTypeError(*this);
    }
    
    TypeVal DcmTypeError::expected() {
        return exp;
    }
    
    TypeVal DcmTypeError::received() {
        return got;
    }
    
    TypeVal DcmTypeError::type() {
        static unsigned char typeVal[] = {TYPE_E};
        return typeVal;
    }
    
    string DcmTypeError::repr() {
        return "Error: Type Error expected: , got: ";
    }
// };

// DcmBoundsError {
    DcmBoundsError::DcmBoundsError(DcmBoundsError& toCopy) {
        top = toCopy.top;
        giv = toCopy.giv;
    }
    
    DcmBoundsError::DcmBoundsError(int topBound, int given) {
        top = topBound;
        giv = given;
    }
    
    DcmType *DcmBoundsError::copy() {
        return new DcmBoundsError(*this);
    }
    
    int DcmBoundsError::topBound() {
        return top;
    }
    
    int DcmBoundsError::given() {
        return giv;
    }
    
    TypeVal DcmBoundsError::type() {
        static unsigned char typeVal[] = {OUT_OF_BOUNDS_E};
        return typeVal;
    }
// };

// DcmStackError {
    DcmStackError::DcmStackError(DcmStackError& toCopy) {
        senderType = toCopy.senderType;
        dcmStack = toCopy.dcmStack;
        dcmStack->addRef();
    }
    
    DcmStackError::DcmStackError(DcmSymbol* sym, TypeVal sentFrom) {
        senderType = sentFrom;
        dcmStack = sym;
        dcmStack->addRef();
    }
    
    DcmStackError::~DcmStackError() {
        if (!refs) {
            del(dcmStack);
        }
    }
    
    DcmType *DcmStackError::copy() {
        return new DcmStackError(*this);
    }
    
    TypeVal DcmStackError::sender() {
        return senderType;
    }
    
    DcmSymbol *DcmStackError::getSymbol() {
        return dcmStack;
    }
    
    TypeVal DcmStackError::type() {
        static unsigned char typeVal[] = {STACK_EMPTY_E};
        return typeVal;
    }
    
    string DcmStackError::repr() {
        return "Error: Stack " + dcmStack->get() + " empty";
    }
// };

// DcmCustomError {
    DcmCustomError::DcmCustomError(DcmCustomError& toCopy) {
        msg = toCopy.msg;
    }
    
    DcmCustomError::DcmCustomError(string& message) {
        msg = message;
    }
    
    DcmType *DcmCustomError::copy() {
        return new DcmCustomError(*this);
    }
    
    TypeVal DcmCustomError::subtype() {
        static unsigned char typeVal[] = {0};
        return typeVal;
    }
    
    TypeVal DcmCustomError::type() {
        static TypeVal typeVal = 0; 
        if (!typeVal) {
            int len = 1;
            TypeVal subtypeVal = subtype();
            unsigned char *pos = subtypeVal;
            while (*pos & 1) {
                len++;
                pos++;
            }
            typeVal = new unsigned char[len];
            for (int i = 1; i < len; i++)
                typeVal[i] = subtypeVal[i-1];
            typeVal[0] = ELEM | (EXTEN << 1) | 1;
        }
        return typeVal;
    }
    
    string DcmCustomError::repr() {
        return "Error: UserError: " + msg;
    }
// };
