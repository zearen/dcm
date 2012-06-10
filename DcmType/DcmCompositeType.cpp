/*
    Zachary Weaver
    27/07/2011
    DcmCompositeType.cpp
*/

#include <algorithm>
#include <sstream>

#include "DcmType.h"

using namespace std;
using namespace Dcm;

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
    
    bool DcmArray::equals(DcmType& dcm) {
        DcmArray& other = static_cast<DcmArray&>(dcm);
        if (len != other.len) return false;
        for (int i = 0; i < len; i++) {
            if (*(vals[i]) != *(other.vals[i])) return false;
        }
        return true;
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
        stringstream ss;
        ss << "( ";
        for (int i=0; i < len; i++) {
            ss << vals[i]->repr() << " ";
        }
        ss << ")";
        return ss.str();
    }
    
    int DcmArray::length() {
        return len;
    }
    
    DcmType*& DcmArray::operator[] (int index) throw (DcmBoundsError*) {
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
    
    DcmType *DcmNamespace::restore() {
        return this;
    }
    
    bool DcmNamespace::equals(DcmType& dcm) {
        // Users should just use .= for class equality
        throw new DcmTypeError({}, DcmNamespace::typeVal());
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
    
    Namespace *DcmNamespace::getNamespace() {
        return (DcmNamespace*)dup(this);
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
    
    bool DcmClass::equals(DcmType& dcm) {
        return &dcm == this;
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
    
    DcmClass *DcmClass::base() {
        return dcmBase;
    }

    TypeVal DcmClass::typeVal() {
        static unsigned char typeVal[] = {CLASS};
        return typeVal;
    }
    
    TypeVal DcmClass::type() {
        return DcmClass::typeVal();
    }
    
    string DcmClass::repr() {
        return "<Class@" + to_string((long)this) + ">";
    }
// };

// DcmPrimFun {
    DcmPrimFun::DcmPrimFun() {
        cb = NULL;
    }
    
    DcmPrimFun::DcmPrimFun(DcmPrimFun& toCopy) {
        cb = toCopy.cb;
    }
    
    DcmPrimFun::DcmPrimFun(Callback *action) {
        cb = action;
    }
    
    DcmPrimFun::~DcmPrimFun() {
        if (!refs && cb->mustDestroy()) {
            delete cb;
        }
    }
    
    bool DcmPrimFun::equals(DcmType& dcm) {
        DcmPrimFun& dcmPrim = static_cast<DcmPrimFun&>(dcm);
        return &cb == &dcmPrim.cb;
    }

    // PrimFuns are immutable
    DcmType *DcmPrimFun::copy() {
        return dup(this);
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
        return "<PrimFun@" + to_string((long)this) + ">";
    }
// };

// DcmExec {
    unsigned long DcmExec::count = 0;
    
    DcmExec::DcmExec() {
        num = count;
        count++;
    }
    
    DcmExec::DcmExec(DcmExec& toCopy) 
        : vector<DcmType*>(toCopy) {
        num = count;
        count++;
        for (DcmType *dcm : (*this)) {
            dup(dcm);
        }
    }
    
    DcmExec::~DcmExec() {
        for (DcmType *e : *this) {
            del(e);
        }
    }
    
    bool DcmExec::equals(DcmType& dcm) {
        throw new DcmTypeError({}, DcmExec::typeVal());
    }

    DcmType *DcmExec::copy() {
        return new DcmExec(*this);
    }
    
    TypeVal DcmExec::typeVal() {
        static unsigned char typeVal[] = {EXEC};
        return typeVal;
    }
    
    TypeVal DcmExec::type() {
        return DcmExec::typeVal();
    }
    
    string DcmExec::repr() {
        stringstream ss;
        ss << "[ " << hex << num << " ]";
        return ss.str();
    }
// };
