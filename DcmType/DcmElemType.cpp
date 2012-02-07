/*
    Zachary Weaver
    26/07/2011
    DcmElemType.cpp
*/

#include "DcmType.h"
#include <sstream>

// DcmNone {
    DcmType *DcmNone::copy() {
        return new DcmNone(*this);
    }

    TypeVal DcmNone::typeVal() {
        static unsigned char typeVal[] = {ELEM | (NONE << 1)};
        return typeVal;
    }
    
    TypeVal DcmNone::type() {
        return DcmNone::typeVal();
    }
    
    string DcmNone::repr() {
        return "None";
    }
    
    DcmNone *dcmNone = new DcmNone();
// };

// DcmInt {
    DcmInt::DcmInt(DcmInt& toCopy) {
        val = toCopy.val;
    }
    
    DcmInt::DcmInt(int newVal) {
        val = newVal;
    }
    
    DcmType *DcmInt::copy() {
        return new DcmInt(*this);
    }

    TypeVal DcmInt::typeVal() {
        static unsigned char typeVal[] = {ELEM | (INT << 1)};
        return typeVal;
    }
    
    TypeVal DcmInt::type() {
        return DcmInt::typeVal();
    }
    
    string DcmInt::repr() {
        stringstream sout;
        sout << val;
        return sout.str();
    }
// };

// DcmFloat {
    DcmFloat::DcmFloat(DcmFloat& toCopy) {
        val = toCopy.val;
    }
    
    DcmFloat::DcmFloat(double newVal) {
        val = newVal;
    }
    
    DcmType *DcmFloat::copy() {
        return new DcmFloat(*this);
    }

    TypeVal DcmFloat::typeVal() {
        static unsigned char typeVal[] = {ELEM | (FLOAT << 1)};
        return typeVal;
    }
    
    TypeVal DcmFloat::type() {
        return DcmFloat::typeVal();
    }
    
    string DcmFloat::repr() {
        stringstream sout;
        sout << val;
        return sout.str();
    }
// };

// DcmChar {
    DcmChar::DcmChar(DcmChar& toCopy) {
        val = toCopy.val;
    }
    
    DcmChar::DcmChar(char newVal) {
        val = newVal;
    }
    
    DcmType *DcmChar::copy() {
        return new DcmChar(*this);
    }

    TypeVal DcmChar::typeVal() {
        static unsigned char typeVal[] = {ELEM | (CHAR << 1)};
        return typeVal;
    }
    
    TypeVal DcmChar::type() {
        return DcmChar::typeVal();
    }
    
    string DcmChar::repr() {
        stringstream sout;
        sout << '\'' << val;
        return sout.str();
    }
// };

// DcmBool {
    DcmBool::DcmBool(DcmBool& toCopy) {
        val = toCopy.val;
    }
    
    DcmBool::DcmBool(bool newVal) {
        val = newVal;
    }
    
    DcmType *DcmBool::copy() {
        return new DcmBool(*this);
    }

    TypeVal DcmBool::typeVal() {
        static unsigned char typeVal[] = {ELEM | (BOOL << 1)};
        return typeVal;
    }
    
    TypeVal DcmBool::type() {
        return DcmBool::typeVal();
    }
    
    string DcmBool::repr() {
        if (val)
            return "true";
        else
            return "false";
    }
// };

// DcmString {
    DcmString::DcmString() {}
    
    DcmString::DcmString(string& toCopy) : string(toCopy) {
    }
    
    DcmType *DcmString::copy() {
        return new DcmString(*this);
    }

    TypeVal DcmString::typeVal() {
        static unsigned char typeVal[] = {ELEM | (STRING << 1)};
        return typeVal;
    }
    
    TypeVal DcmString::type() {
        return DcmString::typeVal();
    }
    
    string DcmString::repr() {
        return "\"" + *this + "\"";
    }
// };

// DcmSymbol {
    DcmSymbol::DcmSymbol(DcmSymbol& toCopy) {
        symbol = toCopy.symbol;
    }
    
    DcmSymbol::DcmSymbol(string name) {
        symbol = name;
    }
    
    DcmType *DcmSymbol::copy() {
        return new DcmSymbol(*this);
    }
    
    string DcmSymbol::get() {
        return symbol;
    }
    
    TypeVal DcmSymbol::typeVal() {
        static unsigned char typeVal[] = {ELEM | (SYMBOL << 1)};
        return typeVal;
    }
    
    TypeVal DcmSymbol::type() {
        return DcmSymbol::typeVal();
    }
    
    string DcmSymbol::repr() {
        return "," + symbol;
    }
// };

// DcmExtension {
    TypeVal DcmExtension::type() {
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
// };
