/*
    Zachary Weaver
    23/07/2011
    Callback.h
*/

#ifndef _CALLBACK_H
#define _CALLBACK_H

#include <stack>

class Callback;
class Interpretter;

#include "DcmType/DcmType.h"
#include "Interpretter.h"

using namespace std;

DcmType *raw_peek(string& sym, Scope *scope);

class Callback {
    protected:
        // peek and pop can return stack empty error
        // It is ill advised to override these
        static DcmType *Peek(Scope *scope, string& sym)
          throw (DcmStackError*);
        static DcmType *Pop(Scope *scope, string& sym)
          throw (DcmStackError*);
        static void Push(Scope *scope, string& sym, DcmType *item);
    public:
        string name;
        // If run returns a callback, the interpretter executes it
        // Used primarily for tail call optimization by x
        virtual Callback *run(Interpretter *interpretter) =0;
};

class SimpleCallback : public Callback {
    private:
        void (*cb)(DcmStack&);
    public:
        SimpleCallback(void (*callback)(DcmStack&));
        Callback *run(DcmStack& stk);
        Callback *run(Interpretter *interpretter);
};

class ExecCallback : public Callback {
    private:
        DcmExec *dcmRun;
    public:
        ExecCallback(DcmExec *exec);
        ~ExecCallback();
        Callback *run(Interpretter *interpretter);
};
#endif
