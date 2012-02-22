/*
    Zachary Weaver
    26/07/2011
    DcmType.cpp
*/

#include "DcmType.h"
#include <sstream>

DcmType::DcmType() {
    refs = 1;
}

DcmType::DcmType(DcmType &toCopy) {
    refs = 1;
}

void DcmType::addRef() {
    refs++;
}

int DcmType::refCount() {
    return (int) refs;
}

void del(DcmType *dcm) {
    dcm->refs--;
    if (dcm->refs) {
        dcm = NULL;
    }
    else {
        delete dcm;
    }
}

DcmType *dup(DcmType *dcm) {
    dcm->refs++;
    return dcm;
}

string typeVal2Str(TypeVal type) {
    stringstream ret;
    ret << hex << '<';
    while (*type & 1) {
        ret << *type;
        type++;
    }
    ret << *type << '>';
    return ret.str();
}

bool DcmType::isType(TypeVal otherType) {
    bool same = true;
    unsigned char* myPos = type(), *itsPos = otherType;
    while(true) {
        same &= *myPos == *itsPos;
        if (!(*myPos & 1 && *itsPos & 1)) {
            break;
        }
        myPos++;
        itsPos++;
    }
    return same;
}

char Namespace::id() {
    return 'b'; // Baseclass
}
