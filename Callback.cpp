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
void callbackLoop(Callback* cb, Interpretter* interpretter) {
    Callback* cbNext;
    while (cb) {
        cbNext = cb->run(interpretter);
        if (cb->mustDestroy())
            delete cb;
        cb = cbNext;
    }
}

void checkTypes(DcmType* dcm, vector<TypeVal> types)
  throw (DcmTypeError*) {
    if (types.size() == 0) return;
    for (auto type : types) {
        if (dcm->isType(type)) return;
    }
    throw new DcmTypeError(types, dcm->type());
}

DcmType *safePeekMain(Interpretter* interpretter, vector<TypeVal> types)
  throw (DcmError*) {
    return safePeekMain(interpretter->mainStack, types);
}

DcmType *safePeekMain(Interpretter* interpretter) throw (DcmError*) {
    return safePeekMain(interpretter->mainStack, {});
}

DcmType *safePeekMain(DcmStack& stk, vector<TypeVal> types)
  throw (DcmError*) {
   DcmType *dcm;
    if (!stk.empty()) {
        dcm = stk.top();
    }
    else {
        throw new DcmStackError(new DcmSymbol("main stack"),
            DcmPrimFun::typeVal());
    }
    checkTypes(dcm, types);
    return dcm;
}

DcmType *safePeekMain(DcmStack& stk) throw (DcmError*) {
    return safePeekMain(stk, {});
}

DcmType **popN(DcmStack& stk, unsigned int n) {
    int pos = 0;
    DcmType **rets = new DcmType*[n];
    for (; pos < n; pos++) {
        try {
            rets[pos] = safePeekMain(stk);
            stk.pop();
        }
        catch (DcmStackError *ex) {
            // Let's free all the stuff we took first
            for(pos--; pos >= 0; pos--) {
                del(rets[pos]);
            }
            delete rets;
            throw ex;
        }
    }
    return rets;
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

    bool Callback::mustDestroy() {
        return false;
    }
// };

// SimpleCallback {
    SimpleCallback::SimpleCallback(function<void(DcmStack&)>callback) {
        cb = callback;
    }
    
    Callback *SimpleCallback::run(Interpretter *interpretter) {
        cb(interpretter->mainStack);
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
                callbackLoop(cb, interpretter);
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

    bool ExecCallback::mustDestroy() {
        return true;
    }
// };
