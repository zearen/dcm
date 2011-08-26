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

void DcmType::addRef() {
    refs++;
}
