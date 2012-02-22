/*
    Zachary Weaver
    27/08/2011
    Callback.cpp
*/

#include "Callback.h"

static unsigned char type[] = {0};

DcmType *raw_peek(string& sym, Scope *scope) {
    Namespace *ns;
    DcmType *ret = NULL;
    DcmStack *stk;
    if (scope->empty()) {
        return NULL;
    }
    else {
        ns = scope->top();
        scope->pop();
        // if is a DcmClass ...
        if (ns->id() == 'c') {
            ret = static_cast<DcmClass*>(ns)->peek(sym);
            if (!ret) {
                ret = raw_peek(sym, scope);
            }
        }
        else {
            stk = &(*ns)[sym];
            if (stk->empty()) {
                ret = raw_peek(sym, scope);
            }
            else {
                ret = stk->top();
            }
        }
        scope->push(ns);
        return ret;
    }
}

// Utilities
    Callback *addName(Callback *cb, string name) {
        cb->name = name;
        return cb;
    }

DcmType *safePeekMain(Interpretter* interpretter, TypeVal type)
  throw (DcmError*) {
    return safePeekMain(interpretter->mainStack, type);
}

DcmType *safePeekMain(DcmStack& stk, TypeVal type)
  throw (DcmError*) {
    DcmType *dcm;
    if (!stk.empty()) {
        dcm = stk.top();
    }
    else {
        throw new DcmStackError(new DcmSymbol("main stack"),
            DcmPrimFun::typeVal());
    }
    if (type && dcm->isType(type))
        throw new DcmTypeError(type, dcm->type());
    return dcm;
}

// Callback {
    DcmType *Callback::Peek(Scope *scope, string& sym) 
      throw (DcmStackError*) {
        DcmType *ret=NULL;
        if (ret = raw_peek(sym, scope)) {
            return ret;
        }
        else {
            throw new DcmStackError(new DcmSymbol(sym), type);
        }
    }
    
    DcmType *Callback::Pop(Scope *scope, string& sym) 
      throw (DcmStackError*) {
        DcmStack *stk;
        DcmType *ret;
        stk = &(*scope->top())[sym];
        if (stk->empty()) {
            throw new DcmStackError(new DcmSymbol(sym), type);
        }
        else {
            ret = stk->top();
            stk->pop();
            return ret;
        }
    }
    
    void Callback::Push(Scope *scope, string& sym, DcmType *item){
        (*scope->top())[sym].push(item);
    }
// };

// SimpleCallback {
    SimpleCallback::SimpleCallback(void (*callback)(DcmStack&)) {
        cb = callback;
    }
    
    Callback *SimpleCallback::run(Interpretter *interpretter) {
        (*cb)(interpretter->mainStack);
        return NULL;
    }
// };

// ExecCallback {
    ExecCallback::ExecCallback(DcmExec *exec) {
        dcmRun = exec;
        dcmRun->addRef();
    }
    
    ExecCallback::~ExecCallback() {
        del(dcmRun);
    }
    
    Callback *ExecCallback::run(Interpretter *interpretter) {
        if (dcmRun->size() == 0) {
            return NULL;
        }

        auto end = dcmRun->end();
        end--;

        for (auto i = dcmRun->begin(); i != end; i++) {
            if ((*i)->isType(DcmPrimFun::typeVal())) {
                Callback *cb = static_cast<DcmPrimFun*>(*i)
                    ->run(interpretter);
                while (cb = cb->run(interpretter));
            }
            else {
                interpretter->mainStack.push(dup(*i));
            }
        }

        if ((*end)->isType(DcmPrimFun::typeVal())) {
            return static_cast<DcmPrimFun*>(*end)
                ->run(interpretter);
        }
        else {
            interpretter->mainStack.push(dup(*end));
            return NULL;
        }
    }
// };
