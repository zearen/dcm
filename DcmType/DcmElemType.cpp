/*
    Zachary Weaver
    26/07/2011
    DcmElemType.cpp
*/

#include "DcmType.h"

// DcmNone {
    TypeVal DcmNone::type() {
        static unsigned char typeVal[] = {ELEM | (NONE << 1)};
        return typeVal;
    }
    
    string DcmNone::repr() {
        return "None";
    }
// };

// DcmInt {
    DcmInt::DcmInt(DcmInt& toCopy) {
        val = toCopy.val;
    }
    
    DcmInt::DcmInt(int newVal) {
        val = newVal;
    }
    
    TypeVal DcmInt::type() {
        static unsigned char typeVal[] = {ELEM | (INT << 1)};
        return typeVal;
    }
    
    string DcmInt::repr() {
        return "Int";
    }
// };

// DcmFloat {
    DcmFloat::DcmFloat(DcmFloat& toCopy) {
        val = toCopy.val;
    }
    
    DcmFloat::DcmFloat(double newVal) {
        val = newVal;
    }
    
    TypeVal DcmFloat::type() {
        static unsigned char typeVal[] = {ELEM | (FLOAT << 1)};
        return typeVal;
    }
    
    string DcmFloat::repr() {
        return "Float";
    }
// };

// DcmChar {
    DcmChar::DcmChar(DcmChar& toCopy) {
        val = toCopy.val;
    }
    
    DcmChar::DcmChar(char newVal) {
        val = newVal;
    }
    
    TypeVal DcmChar::type() {
        static unsigned char typeVal[] = {ELEM | (CHAR << 1)};
        return typeVal;
    }
    
    string DcmChar::repr() {
        return "Char";
    }
// };

// DcmBool {
    DcmBool::DcmBool(DcmBool& toCopy) {
        val = toCopy.val;
    }
    
    DcmBool::DcmBool(bool newVal) {
        val = newVal;
    }
    
    TypeVal DcmBool::type() {
        static unsigned char typeVal[] = {ELEM | (BOOL << 1)};
        return typeVal;
    }
    
    string DcmBool::repr() {
        if (val)
            return "true";
        else
            return "false";
    }
// };

// DcmString {
    TypeVal DcmString::type() {
        static unsigned char typeVal[] = {ELEM | (STRING << 1)};
        return typeVal;
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
    
    string DcmSymbol::get() {
        return symbol;
    }
    
    TypeVal DcmSymbol::type() {
        static unsigned char typeVal[] = {ELEM | (SYMBOL << 1)};
        return typeVal;
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
