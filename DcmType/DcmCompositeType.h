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
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
        int length();
        DcmType *operator[] (int index) throw (DcmBoundsError*);
};

class DcmNamespace : public DcmType, public Namespace {
    public:
        char id();
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

class DcmClass : public DcmNamespace {
    private:
        DcmClass *dcmBase;
    public:
        char id();
        DcmClass();
        DcmClass(DcmClass& toCopy);
        DcmClass(DcmClass *baseClass);
        ~DcmClass();
        bool equals(DcmType& dcm);
        DcmType *copy();
        DcmType *peek(string& sym);
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

class DcmPrimFun : public DcmType {
    private:
        Callback *cb;
    public:
        DcmPrimFun();
        DcmPrimFun(DcmPrimFun& toCopy);
        DcmPrimFun(Callback *action);
        ~DcmPrimFun();
        bool equals(DcmType& dcm);
        DcmType *copy();
        Callback *run(Interpretter *interpretter);
        Callback *callback();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};

class DcmExec : public DcmType, public vector<DcmType*> {
    private:
        // Total count of execs
        static unsigned long count;
        // Number for when exec was created
        // Note:  Not garanteed to be unique !
        unsigned long num;
    public:
        DcmExec();
        DcmExec(DcmExec& toCopy);
        ~DcmExec();
        bool equals(DcmType& dcm);
        DcmType *copy();
        TypeVal type();
        static TypeVal typeVal();
        string repr();
};
