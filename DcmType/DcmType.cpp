/*
    Zachary Weaver
    26/07/2011
    DcmType.cpp
*/

#include "DcmType.h"
#include <sstream>

char Namespace::id() {
    return 'b'; // Baseclass
}

DcmType *Namespace::restore() {
    return NULL;
}

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

Namespace* DcmType::getNamespace() {
    return NULL;
}

bool DcmType::operator==(DcmType& dcm) {
    if (this->isType(dcm.type())) return this->equals(dcm);
    else return false;
}

bool DcmType::operator!=(DcmType& dcm) {
    return !(*this == dcm);
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
        ret << (short)*type;
        type++;
    }
    ret << (short)*type << '>';
    return ret.str();
}

bool sameType(TypeVal type1, TypeVal type2) {
    bool same = true;
    while(true) {
        same &= *type1 == *type2;
        if (!(*type1 & 1 && *type2 & 1)) {
            break;
        }
        type1++;
        type2++;
    }
    return same;
}

bool DcmType::isType(TypeVal otherType) {
    return sameType(type(), otherType);
}

