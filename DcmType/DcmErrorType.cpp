/*
    Zachary Weaver
    
    DcmErrorType.cpp
*/

#include "DcmType.h"
#include <sstream>

// DcmError {
    DcmType *DcmError::copy() {
        return new DcmError(*this);
    }
    
    TypeVal DcmError::typeVal() {
        static unsigned char typeVal[] = {UNKNOWN_E};
        return typeVal;
    }
    
    TypeVal DcmError::type() {
        return DcmError::typeVal();
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
    
    DcmTypeError::DcmTypeError(vector<TypeVal> expected, TypeVal received) {
        exp = expected;
        got = received;
    }
    
    DcmType *DcmTypeError::copy() {
        return new DcmTypeError(*this);
    }
    
    vector<TypeVal> DcmTypeError::expected() {
        return exp;
    }
    
    TypeVal DcmTypeError::received() {
        return got;
    }
    
    TypeVal DcmTypeError::typeVal() {
        static unsigned char typeVal[] = {TYPE_E};
        return typeVal;
    }
    
    TypeVal DcmTypeError::type() {
        return DcmTypeError::typeVal();
    }
    
    string DcmTypeError::repr() {
        stringstream ret;
        ret << "Error: TypeError expected: ( ";
        for (auto e : exp)
            ret << typeVal2Str(e) << " ";
        ret << "), got: ";
        ret << typeVal2Str(got);
        return ret.str();
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
    
    TypeVal DcmBoundsError::typeVal() {
        static unsigned char typeVal[] = {OUT_OF_BOUNDS_E};
        return typeVal;
    }
    
    TypeVal DcmBoundsError::type() {
        return DcmBoundsError::typeVal();
    }
    
    string DcmBoundsError::repr() {
        stringstream ret;
        ret << "Error: Out of bounds (";
        ret << giv << " > " << top << ")";
        return ret.str();
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
    
    TypeVal DcmStackError::typeVal() {
        static unsigned char typeVal[] = {STACK_EMPTY_E};
        return typeVal;
    }
    
    TypeVal DcmStackError::type() {
        return DcmStackError::typeVal();
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
            typeVal[0] = ERROR | (CUSTOM_E << 1) | 1;
        }
        return typeVal;
    }
    
    TypeVal DcmCustomError::typeVal() {
        static unsigned char typeVal[] = {ERROR | (CUSTOM_E << 1) | 1, 0};
        return typeVal;
    }
    
    string DcmCustomError::repr() {
        return "Error: UserError: " + msg;
    }
// };
