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

#include <functional>

#include "DcmType/DcmType.h"
#include "Interpretter.h"

using namespace std;

DcmType *raw_peek(string& sym, Scope *scope);

// Executes a callback until a NULL is encountered
void callbackLoop(Callback* cb, Interpretter* interpretter);

// Checks to see if dcm is one of the given types.
// Will throw error if not.
void checkTypes(DcmType* dcm, vector<TypeVal> types)
  throw (DcmTypeError*);

// Peeks the top item of the stack throwing errors if the stack is empty
// Optionally, one can specify the necessary type of the value
DcmType *safePeekMain(Interpretter* interpretter, vector<TypeVal> types)
  throw (DcmError*);

DcmType *safePeekMain(Interpretter* interpretter) throw (DcmError*);

DcmType *safePeekMain(DcmStack& stk, vector<TypeVal> types)
  throw (DcmError*);

DcmType *safePeekMain(DcmStack& stk) throw (DcmError*);

// Safely pops n things from the stack and returns them in an array
// Manages most garbage for you, but you must delete array
// The elements are added in the order the were popped
// Does nothing to the reference count if no error
DcmType **popN(DcmStack& stk, unsigned int n);

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
        virtual ~Callback(){}
        // If run returns a callback, the interpretter executes it
        // Used primarily for tail call optimization by x
        virtual Callback *run(Interpretter *interpretter) =0;
        // returns whether the callback should be destroyed after
        // execution
        virtual bool mustDestroy();
};

class SimpleCallback : public Callback {
    private:
        function<void(DcmStack&)> cb;
    public:
        SimpleCallback(function<void(DcmStack&)>callback);
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
        bool mustDestroy();
};
#endif
