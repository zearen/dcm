/*
    Zachary Weaver
    27/08/2011
    Callback.cpp
*/


#include "Callback.h"

static unsigned char type[] = {0};

DcmType *peek(string& sym, stack<Namespace*> *scope) {
    Namespace *ns;
    DcmType *ret;
    DcmClass *dcmClass;
    if (scope->empty()) {
        return NULL;
    }
    else {
        ns = scope->pop();
        if (dcmClass = dynamic_cast<*DcmClass>(ns)) {
            ret = dcmClass->peek(sym);
        }
        else if {
            ret = (*ns)[sym];
        }
        else if (ret.empty()) {
            ret = peek(sym, scope);
        }
        scope->push(ns);
        return ret;
    }
}

// Callback {
    Callback::Callback() {
        scope = NULL;
    }
    
    Callback::Callback(stack<Namespace*> *vars){
        scope = vars;
    }
    
    DcmType *Callback::peek(string& sym) throw (DcmStackError*) {
        DcmType *ret=NULL;
        if (ret = peek(sym, scope)) {
            return ret;
        }
        else {
            throw new DcmStackError(sym, type);
        }
    }
    
    DcmType *Callback::pop(string& sym) throw (DcmStackError*) {
        DcmStack *stk;
        stk = &(*scope->top())[sym]
        if (stk->empty()) {
            throw new DcmStackError(sym, type);
        }
        else {
            return stk->pop();
        }
    }
    
    void Callback::push(string& sym, DcmType *item){
        (*scope->top())[sym].push(item);
    }
// };


