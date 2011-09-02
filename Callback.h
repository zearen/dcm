/*
    Zachary Weaver
    23/07/2011
    Callback.h
*/
#ifndef _CALLBACK_H
#define _CALLBACK_H

#include <stack>

class Callback;

#include "DcmType/DcmType.h"
//#include "Interpretter.h"

typedef stack<Namespace*> Scope;

using namespace std;

DcmType *raw_peek(string& sym, Scope *scope);

class Callback {
    protected:
        // peek and pop can return stack empty error
        // It is ill advised to override these
        DcmType *peek(string& sym) throw (DcmStackError*);
        DcmType *pop(string& sym) throw (DcmStackError*);
        void push(string& sym, DcmType *item);
        Scope *scope;
        unordered_map<string, Callback*> heaven;
    public:
        string name;
        Callback();
        //void connect(Interpreter *interpretter);
        // If run returns a callback, the interpretter executes it
        // Used primarily for tail call optimization by x
        virtual Callback *run(DcmStack& stk) =0;
};

class SimpleCallback : public Callback {
    private:
        void (*cb)(DcmStack&);
    public:
        SimpleCallback(void (*callback)(DcmStack&));
        Callback *run(DcmStack& stk);
};

class ExecCallback : public Callback {
    private:
        DcmExec *dcmRun;
    public:
        ExecCallback(DcmExec *exec);
        ~ExecCallback();
        Callback *run(DcmStack& stk);
};
#endif
