/*
    Zachary Weaver: zaw6@pitt.edu
    17/02/2011
    DcmCompositeType.h
*/

#ifndef DCM_MAIN
#include "DcmType.h"
#endif

#include "../Callback.h"

class DcmArray : public DcmType {
    protected:
        int len;
        DcmType ** vals; 
    public:
        DcmArray();
        DcmArray(DcmArray& toCopy);
        DcmArray(int size);
        ~DcmArray();
        TypeVal type();
        string repr();
        int length();
        DcmType *operator[] (int index);
};

class DcmNamespace : public DcmType, public Namespace {
    public:
        DcmNamespace(DcmNamespace& toCopy);
        TypeVal type();
        string repr();
};

class DcmClass : public DcmNamespace {
    private:
        DcmClass *dcmBase;
    public:
        DcmClass(DcmClass& toCopy);
        DcmClass(DcmClass *baseClass);
        TypeVal type();
        string repr();
};

class DcmPrimFun : public DcmType {
    private:
        Callback *cb;
    public:
        DcmPrimFun(DcmPrimFun& toCopy);
        DcmPrimFun(Callback *action);
        DcmPrimFun();
        Callback *run(DcmStack& stk);
        TypeVal type();
        string repr();
};

class DcmExec : public DcmType {
    private:
        vector<DcmType*> instructions;
    public:
        DcmExec(DcmExec& toCopy);
        DcmExec();
        void append(DcmType* instruction);
        DcmType *operator[](int i);
        TypeVal type();
        string repr();
};
