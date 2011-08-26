/*
    Zachary Weaver
    27/07/2011
    DcmCompositeType.h
*/

#include "DcmType.h"

DcmArray::DcmArray() {
    len = 0;
}

DcmArray::DcmArray(DcmArray& toCopy) {
    len = toCopy.len;
    vals = new DcmType*[len];
    for (int i = 0; i < len; i++) {
        vals[i] = toCopy.vals[i];
        vals[i]->addRef();
    }
}

DcmArray::DcmArray(int size) {
    len = size;
    vars = new DcmType*[len];
    for (int i = 0; i < len; i++) {
        dcmNone->addRef();
        vars[i] = dcmNone;
    }
}

DcmArray::~DcmArray() {
    for (int i = 0; i < len; i++)
        delete vars[i];
    delete[] vars;
}

TypeVal DcmArray::type() {
    static unsigned char typeVal[] = {ARRAY};
    return typeVal;
}

string DcmArray::repr() {
    string ret = "(";
    bool comma = false;
    
}

int DcmArray::length() {
    return len;
}

DcmType *DcmArray::operator[] (int index) throw DcmBoundsError(){
    if (index < 0 || index >= len)
        throw DcmBoundsError(len - 1, index);
    return vars[index];
}
