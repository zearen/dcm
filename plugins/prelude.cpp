#include "prelude.h"

#include <iostream>

void cbDup(DcmStack& stk) {
    stk.push(dup(safePeekMain(stk)));
}

void cbDel(DcmStack& stk) {
    del(safePeekMain(stk));
    stk.pop();
}

void cbClear(DcmStack& stk) {
    while (!stk.empty()) {
        del(stk.top());
        stk.pop();
    }
}

class : public Callback {
    Callback *run(Interpretter *interpretter) {
        DcmType *dcm = safePeekMain(interpretter);
        Callback *cbRet = NULL;
        interpretter->mainStack.pop();
        if (dcm->isType(DcmPrimFun::typeVal())) {
            cbRet = static_cast<DcmPrimFun*>(dcm)->run(interpretter);
        }
        else if (dcm->isType(DcmExec::typeVal())) {
            cbRet = new ExecCallback(static_cast<DcmExec*>(dcm));
        }
        else {
            del(dcm);
            throw new DcmTypeError(DcmExec::typeVal(), dcm->type());
        }
        del(dcm);
        return cbRet;
    }
} cbX;

void cbRefs(DcmStack& stk) {
    DcmType *dcm = safePeekMain(stk);
    stk.push(new DcmInt(dcm->refCount()));
}

Plugin *preludePlugin() {
    vector<Callback*> v = 
        { addName(new SimpleCallback(cbDup), "dup")
        , addName(new SimpleCallback(cbDel), "del")
        , addName(new SimpleCallback(cbClear), "clear")
        , addName(&cbX, "x")
        , addName(new SimpleCallback(cbRefs), "refs")
        };
    return new VectorPlugin(v);
}
