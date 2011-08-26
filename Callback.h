/*
    Zachary Weaver
    23/07/2011
    Callback.h
*/
#ifndef _CALLBACK_H
#define _CALLBACK_H

#include <stack>

#include "DcmType/DcmType.h"

using namespace std;

class Callback {
    private:
    protected:
        // peek and pop can return stack empty error
        // It is ill advised to override these
        DcmType *peek(string& sym);
        DcmType *pop(string& sym);
        void push(string& sym, DcmType *item);
        stack<Namespace*> *scope;
    public:
        string name;
        Callback(stack<Namespace*> *vars);
        // If run returns a callback, the interpretter executes it
        // Used primarily for tail call optimization by x
        virtual Callback *run(DcmStack& stk) =0;
};

class SimpleCallback : public Callback {
    public:
        SimpleCallback(stack<Namespace*> *vars, void (*cb)(DcmStack&));
        Callback *run(DcmStack& stk);
};

class ExecCallback : public Callback {
    private:
        DcmExec *dcmRun;
    public:
        ExecCallback(stack<Namespace*> *vars, DcmExec *exec);
        ~ExecCallback();
        Callback *run(DcmStack& stk);
        void operator delete(void *p);
};
#endif
