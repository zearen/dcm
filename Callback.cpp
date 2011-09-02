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
        DcmType *dcm;
        for (auto i = dcmRun->begin();  i != end; i++) {
            dcm = *i;
            if (*dcm->type() = PRIMFUN) {
                static_cast<DcmPrimFun*>(dcm)->run(interpretter);
            }
            else {
                interpretter->mainStack.push(dcm->copy());
            }
        }
        if (*(*end)->type() = PRIMFUN) {
            return static_cast<DcmPrimFun*>(*end)->callback();
        }
        else {
            return NULL;
        }
    }
// };
