#include "prelude.h"
#include "preludeMath.h"

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
            TypeVal type = dcm->type();
            del(dcm);
            throw new DcmTypeError(
                {DcmExec::typeVal(), DcmPrimFun::typeVal()}, type);
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
      {DcmExec::typeVal()}));
    stk.pop();
    stk.push(new DcmPrimFun(new ExecCallback(exec)));
    // We took it off the stack
    del(exec);
}

// Eventually we'll want ExecStrCallback("$a $b @a @b");
void cbRev(DcmStack& stk) {
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

void cbEquals(DcmStack& stk) {
    DcmType **dcms = popN(stk, 2);
    stk.push(new DcmBool(*dcms[0] == *dcms[1]));
    del(dcms[0]); del(dcms[1]);
    delete dcms;
}

void cbNotEquals(DcmStack& stk) {
    DcmType **dcms = popN(stk, 2);
    stk.push(new DcmBool(*dcms[0] != *dcms[1]));
    del(dcms[0]); del(dcms[1]);
    delete dcms;
}

DcmBool *dcmTrue = new DcmBool(true);
DcmBool *dcmFalse = new DcmBool(false);
DcmNone *dcmNone = new DcmNone();

void cbTrue(DcmStack& stk) {
    stk.push(dup(dcmTrue));
}

void cbFalse(DcmStack& stk) {
    stk.push(dup(dcmFalse));
}

void cbNone(DcmStack& stk) {
    stk.push(dup(dcmNone));
}

void cbType(DcmStack& stk) {
    DcmType *dcm = safePeekMain(stk);
    stk.push(new DcmString(typeVal2Str(dcm->type())));
}

Plugin *preludePlugin() {
    vector<NamedCB> v = 
        // Flow control
        { NamedCB("x",      &cbX)
        , NamedCB("def",    new SimpleCallback(cbDef))
        // Stack management
        , NamedCB("dup",    new SimpleCallback(cbDup))
        , NamedCB("del",    new SimpleCallback(cbDel))
        , NamedCB("clear",  new SimpleCallback(cbClear))
        , NamedCB("refs",   new SimpleCallback(cbRefs))
        , NamedCB("rev",    new SimpleCallback(cbRev))
        , NamedCB("copy",   new SimpleCallback(cbCopy))
        // Types and pseudo-literals
        , NamedCB("true",   new SimpleCallback(cbTrue))
        , NamedCB("false",  new SimpleCallback(cbFalse))
        , NamedCB("none",   new SimpleCallback(cbNone))
        , NamedCB("type",   new SimpleCallback(cbType))
        // Comparison
        , NamedCB("is",     new SimpleCallback(cbIs))
        , NamedCB("=",      new SimpleCallback(cbEquals))
        , NamedCB("/=",     new SimpleCallback(cbNotEquals))
        , NamedCB("<",      new SimpleCallback(cbLT))
        , NamedCB(">",      new SimpleCallback(cbGT))
        , NamedCB("<=",     new SimpleCallback(cbLTE))
        , NamedCB(">=",     new SimpleCallback(cbGTE))
        // Arithmetic
        , NamedCB("+",      new SimpleCallback(cbAdd))
        , NamedCB("-",      new SimpleCallback(cbSub))
        , NamedCB("*",      new SimpleCallback(cbMul))
        , NamedCB("/",      new SimpleCallback(cbDiv))
        , NamedCB("%",      new SimpleCallback(cbMod))
        , NamedCB(">>",     new SimpleCallback(cbRShift))
        , NamedCB("<<",     new SimpleCallback(cbLShift))
        // Boolean operators
        , NamedCB("or",     new SimpleCallback(cbOr))
        , NamedCB("and",    new SimpleCallback(cbAnd))
        , NamedCB("xor",    new SimpleCallback(cbXor))
        , NamedCB("not",    new SimpleCallback(cbNot))
        };
    return new VectorPlugin(v);
}
