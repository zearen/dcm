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

void cbEmpty(DcmStack& stk) {
    stk.push(new DcmBool(stk.empty()));
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
            interpretter->mainStack.push(dcm);
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

void cbCopy(DcmStack& stk) {
    DcmType *dcm = safePeekMain(stk);
    stk.pop();
    stk.push(dcm->copy());
    del(dcm);
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

void cbRepr(DcmStack& stk) {
    DcmType *dcm = safePeekMain(stk);
    stk.pop();
    stk.push(new DcmString(dcm->repr()));
}

void prelude_addBasic(vector<NamedCB>& vec) {
    vector<NamedCB> v = 
        { NamedCB("ex",     &cbX)
        , NamedCB("def",    new SimpleCallback(cbDef))
        // Stack management
        , NamedCB("dup",    new SimpleCallback(cbDup))
        , NamedCB("del",    new SimpleCallback(cbDel))
        , NamedCB("clear",  new SimpleCallback(cbClear))
        , NamedCB("empty",  new SimpleCallback(cbEmpty))
        , NamedCB("refs",   new SimpleCallback(cbRefs))
        , NamedCB("rev",    new SimpleCallback(cbRev))
        , NamedCB("copy",   new SimpleCallback(cbCopy))
        // Types and pseudo-literals
        , NamedCB("true",   new SimpleCallback(cbTrue))
        , NamedCB("false",  new SimpleCallback(cbFalse))
        , NamedCB("none",   new SimpleCallback(cbNone))
        , NamedCB("type",   new SimpleCallback(cbType))
        , NamedCB("repr",   new SimpleCallback(cbRepr))
        };
    vec.insert(vec.end(), v.begin(), v.end());
}

Plugin *preludePlugin() {
    vector<NamedCB> v;
    prelude_addBasic(v);
    prelude_addControl(v);
    prelude_addMath(v);
    //prelude_addOOP(v);
    return new VectorPlugin(v);
}

ExecRunner::ExecRunner() : ExecCallback() {
}

// The idea here is that when we're done with the Runner
// we're done with the exec.
ExecRunner::ExecRunner(DcmExec* exec) : ExecCallback() {
    setExec(exec);
}

void ExecRunner::setExec(DcmExec *exec) {
    if (dcmRun) del(dcmRun);
    dcmRun = exec;
}

// Run the exec
void ExecRunner::runExec(Interpretter *interpretter) {
    if (dcmRun)
        callbackLoop(run(interpretter), interpretter);
}

bool ExecRunner::mustDestroy() {
    return false;
}

