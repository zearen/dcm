/*
    Zachary Weaver
    27/08/2011
    Callback.cpp
*/

#include "Callback.h"

static unsigned char type[] = {0};

DcmType *raw_peek(string& sym, stack<Namespace*> *scope) {
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
    Callback::Callback() {
        scope = NULL;
    }
    
    Callback::Callback(stack<Namespace*> *vars){
        scope = vars;
    }
    
    DcmType *Callback::peek(string& sym) throw (DcmStackError*) {
        DcmType *ret=NULL;
        if (ret = raw_peek(sym, scope)) {
            return ret;
        }
        else {
            throw new DcmStackError(new DcmSymbol(sym), type);
        }
    }
    
    DcmType *Callback::pop(string& sym) throw (DcmStackError*) {
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
    
    void Callback::push(string& sym, DcmType *item){
        (*scope->top())[sym].push(item);
    }
// };

// SimpleCallback {
    SimpleCallback::SimpleCallback(stack<Namespace*> *vars, void (*callback)(DcmStack&)) {
        cb = callback
        scope = vars;
    }
    
    Callback *SimpleCallback::run(DcmStack& stk) {
        (*cb)(stk);
        return NULL;
    }
// }

// ExecCallback {
    ExecCallback::ExecCallback(stack<Namespace*> *vars, DcmExec *exec) {
        scope = vars;
        dcmRun = exec;
        dcmRun->addRef();
    }
    
    ExecCallback::~ExecCallback() {
        del(dcmRun);
    }
    
    Callback *ExecCallback::run(DcmStack& stk) {
        //if (dcmRun
        //TODO: Fnish me, you may need to revamp DcmExec.
    }
