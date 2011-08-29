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
        DcmType *operator[] (int index) throw (DcmBoundsError*);
};

class DcmNamespace : public DcmType, public Namespace {
    public:
        TypeVal type();
        string repr();
};

class DcmClass : public DcmNamespace {
    private:
        DcmClass *dcmBase;
    public:
        DcmClass();
        DcmClass(DcmClass& toCopy);
        DcmClass(DcmClass *baseClass);
        ~DcmClass();
        TypeVal type();
        string repr();
};

class DcmPrimFun : public DcmType, public Callback {
    private:
        Callback *cb;
        bool resp;
    public:
        DcmPrimFun();
        DcmPrimFun(DcmPrimFun& toCopy);
        // By setting responsible, you tie the life of callback 
        //+ to the primfun
        DcmPrimFun(Callback *action, bool responsible=false);
        ~DcmPrimFun();
        Callback *run(DcmStack& stk);
        TypeVal type();
        string repr();
};

class DcmExec : public DcmType {
    private:
        vector<DcmType*> instructions;
        string source;
    public:
        DcmExec();
        DcmExec(string sourceStr);
        DcmExec(DcmExec& toCopy);
        ~DcmExec();
        void append(DcmType* instruction);
        void append(string bit);
        DcmType *operator[](int i);
        TypeVal type();
        string repr();
};
