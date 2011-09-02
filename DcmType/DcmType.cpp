/*
    Zachary Weaver
    26/07/2011
    DcmType.cpp
*/

#include "DcmType.h"

DcmType::DcmType() {
    refs = 1;
}

DcmType::DcmType(DcmType &toCopy) {
    refs = 1;
}

void DcmType::addRef() {
    refs++;
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
