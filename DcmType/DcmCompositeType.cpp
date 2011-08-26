/*
    Zachary Weaver
    27/07/2011
    DcmCompositeType.h
*/

#include "DcmType.h"

// DcmArray {
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
        vals = new DcmType*[len];
        for (int i = 0; i < len; i++) {
            dcmNone->addRef();
            vals[i] = dcmNone;
        }
    }
    
    DcmArray::~DcmArray() {
        for (int i = 0; i < len; i++)
            delete vals[i];
        delete[] vals;
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
    
    DcmType *DcmArray::operator[] (int index) throw (DcmBoundsError*) {
        if (index < 0 || index >= len)
            throw new DcmBoundsError(len - 1, index);
        return vals[index];
    }
// };

// DcmNamespace {
    TypeVal DcmNamespace::type() {
        static unsigned char typeVal[] = {NAMESPACE};
        return typeVal;
    }
    
    string DcmNamespace::repr() {
        return "Namespace @ TBI";
    }
// };

// DcmClass {
    DcmClass::DcmClass() : DcmNamespace() {
        dcmBase = NULL;
    }
    
    DcmClass::DcmClass(DcmClass& toCopy) : DcmNamespace(toCopy) {
        dcmBase = toCopy.dcmBase;
        dcmBase->addRef();
    }
    
    DcmClass::DcmClass(DcmClass* baseClass) {
        dcmBase = baseClass;
        dcmBase->addRef();
    }
    
    DcmClass::~DcmClass() {
        delete dcmBase;
    }
    
    TypeVal DcmClass::type() {
        static unsigned char typeVal[] = {CLASS};
        return typeVal;
    }
    
    string DcmClass::repr() {
        return "Class @ TBI";
    }

// };
