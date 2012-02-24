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

void cbDef(DcmStack& stk) {
    DcmExec *exec = static_cast<DcmExec*>(safePeekMain(stk,
      DcmExec::typeVal()));
    stk.pop();
    stk.push(new DcmPrimFun(new ExecCallback(exec)));
    // We took it off the stack
    del(exec);
}

// Eventually we'll want ExecStrCallback("$a $b @a @b");
void cbR(DcmStack& stk) {
    DcmType **dcms = popN(stk, 2);
    stk.push(dcms[0]);
    stk.push(dcms[1]);
    delete dcms;
}

void cbIs(DcmStack& stk) {
    DcmType **dcms = popN(stk, 2);
    stk.push(new DcmBool(dcms[0] == dcms[1]));
    del(dcms[0]); del(dcms[1]);
    delete dcms;
}

void cbCopy(DcmStack& stk) {
    DcmType *dcm = safePeekMain(stk);
    stk.push(dcm->copy());
}

Plugin *preludePlugin() {
    vector<NamedCB> v = 
        { NamedCB("dup",    new SimpleCallback(cbDup))
        , NamedCB("del",    new SimpleCallback(cbDel))
        , NamedCB("clear",  new SimpleCallback(cbClear))
        , NamedCB("x",      &cbX)
        , NamedCB("refs",   new SimpleCallback(cbRefs))
        , NamedCB("r",      new SimpleCallback(cbR))
        , NamedCB("def",    new SimpleCallback(cbDef))
        , NamedCB("is",     new SimpleCallback(cbIs))
        , NamedCB("copy",   new SimpleCallback(cbCopy))
        };
    return new VectorPlugin(v);
}
