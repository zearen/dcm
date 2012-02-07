/*
    Zachary Weaver
    27/07/2011
    DcmCompositeType.cpp
*/

#include <algorithm>

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
        for (int i = 0; i < len; i++) {
            del(vals[i]);
        }
        delete[] vals;
    }
    
    DcmType *DcmArray::copy() {
        return new DcmArray(*this);
    }
    
    TypeVal DcmArray::typeVal() {
        static unsigned char typeVal[] = {ARRAY};
        return typeVal;
    }
    
    TypeVal DcmArray::type() {
        return DcmArray::typeVal();
    }
    
    string DcmArray::repr() {
        string ret = "(";
        bool comma = false;
        
    }
    
    int DcmArray::length() {
        return len;
    }
    
    DcmType *DcmArray::operator[] (int index) throw (DcmBoundsError*) {
        if (index < 0 || index >= len) {
            throw new DcmBoundsError(len - 1, index);
        }
        return vals[index];
    }
// };

// DcmNamespace {
    char DcmNamespace::id() {
        return 'n'; // dcmNamespace
    }
    
    DcmType *DcmNamespace::copy() {
        return new DcmNamespace(*this);
    }
    
    TypeVal DcmNamespace::typeVal() {
        static unsigned char typeVal[] = {NAMESPACE};
        return typeVal;
    }
    
    TypeVal DcmNamespace::type() {
        return DcmNamespace::typeVal();
    }
    
    string DcmNamespace::repr() {
        return "Namespace @ " + to_string((long)this);
    }
// };

// DcmClass {
    char DcmClass::id() {
        return 'c'; // dcmClass
    }
    
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
        del(dcmBase);
    }
    
    DcmType *DcmClass::copy() {
        return new DcmClass(*this);
    }
    
    DcmType *DcmClass::peek(string& sym) {
        DcmStack *stk;
        stk = &(*this)[sym];
        if (!stk->empty()) {
            return stk->top();
        }
        else {
            if (dcmBase) {
                return dcmBase->peek(sym);
            }
            else {
                return NULL;
            }
        }
    }
    
    TypeVal DcmClass::typeVal() {
        static unsigned char typeVal[] = {CLASS};
        return typeVal;
    }
    
    TypeVal DcmClass::type() {
        return DcmClass::typeVal();
    }
    
    string DcmClass::repr() {
        return "Class @ " + to_string((long)this);
    }
// };

// DcmPrimFun {
    DcmPrimFun::DcmPrimFun() {
        cb = NULL;
    }
    
    DcmPrimFun::DcmPrimFun(DcmPrimFun& toCopy) {
        cb = toCopy.cb;
    }
    
    DcmPrimFun::DcmPrimFun(Callback *action, bool responsible) {
        cb = action;
        resp = responsible;
    }
    
    DcmPrimFun::~DcmPrimFun() {
        if (!refs && resp) {
            delete cb;
        }
    }
    
    DcmType *DcmPrimFun::copy() {
        return new DcmPrimFun(*this);
    }
    
    Callback *DcmPrimFun::run(Interpretter* interpretter) {
        return cb->run(interpretter);
    }
    
    Callback *DcmPrimFun::callback() {
        return cb;
    }
    
    TypeVal DcmPrimFun::typeVal() {
        static unsigned char typeVal[] = {PRIMFUN};
        return typeVal;
    }
    
    TypeVal DcmPrimFun::type() {
        return DcmPrimFun::typeVal();
    }
    
    string DcmPrimFun::repr() {
        return "PrimFun @ " + to_string((long)this);
    }
// };

// DcmExec {
    DcmExec::DcmExec() {
        source = "";
    }
    
    DcmExec::DcmExec(DcmExec& toCopy) : vector<DcmType*>(toCopy) {
        source = toCopy.source;
    }
    
    DcmExec::DcmExec(string& sourceStr) {
        source = sourceStr;
    }
    
    DcmExec::~DcmExec() {
        for (DcmType *e : *this) {
            del(e);
        }
    }
    
    DcmType *DcmExec::copy() {
        return new DcmExec(*this);
    }
    
    void DcmExec::append(string& bit) {
        source += bit;
    }
    
    TypeVal DcmExec::typeVal() {
        static unsigned char typeVal[] = {EXEC};
        return typeVal;
    }
    
    TypeVal DcmExec::type() {
        return DcmExec::typeVal();
    }
    
    string DcmExec::repr() {
        return "[ " + source + " ]";
    }
// };
