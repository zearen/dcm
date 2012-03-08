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

void cbIs(DcmStack& stk) {
    DcmType **dcms = popN(stk, 2);
    stk.push(new DcmBool(dcms[0] == dcms[1]));
    del(dcms[0]); del(dcms[1]);
    delete dcms;
}

void cbCopy(DcmStack& stk) {
    DcmType *dcm = safePeekMain(stk);
    stk.pop();
    stk.push(dcm->copy());
    del(dcm);
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

void cbIf(DcmStack& stk) {
    DcmType **dcms = popN(stk, 3);
    if (dcms[2]->isType(DcmBool::typeVal())) {
        if (static_cast<DcmBool*>(dcms[2])->val)
            stk.push(dup(dcms[1]));
        else
            stk.push(dup(dcms[0]));
    }
    else {
        DcmTypeError *dcmTE new DcmTypeError(
            {DcmBool::typeVal()}, dcms[2]->received());
        stk.push(dcms[2]);
        stk.push(dcms[1]);
        stk.push(dcms[0]);
        delete dcms;
    }
    del(dcms[0]);
    del(dcms[1]);
    del(dcms[2]);
    delete dcms;
}

class ExecRunner : public ExecCallback {
    
    // The idea here is that when we're done with the Runner
    // we're done with the exec.
    ExecRunner(DcmExec* exec) : ExecCallback(exec) {
        del(exec);
    }
    
    // Run the exec
    void runExec(Interpretter *interpretter) {
        callbackLoop(run(interpretter), interpretter);
    }

    bool mustDestroy() {
        return false;
    }
}

class : public Callback {
    Callback *run(Interpretter *interpretter) {
        DcmType **dcms = popN(2, interpretter->mainStack);
        ExecRunner *xBody, *xCond;
        if (!dcms[0]->isType(DcmExec::typeVal())
          || !dcms[1]->isType(DcmExec::typeVal())) {
            DcmTypeError *dcmTE;
            if (!dcms[0]->isType(DcmExec::typeVal())) {
                dcmTE = new DcmTypeError( {DcmExec::typeVal()}
                    dcms[0]->type());
            }
            else {
                dcmTE = new DcmTypeError( {DcmExec::typeVal()}
                    dcms[1]->type());
            }
            interpretter->mainStack.push(dcms[1]);
            interpretter->mainStack.push(dcms[0]);
            delete dcms;
            throw dcmTE;
        }
        delete dcms;  // At this point, the exec garbage collection is
                      //+ handles by RTTI
        
        DcmBool *dcmBool;
        xBody = new ExecRunner(static_cast<DcmExec*>(dcms[0]));
        xCond = new ExecRunner(static_cast<DcmExec*>(dcms[1]));
        xCond.runExec(interpretter);

        dcmBool = static_cast<DcmBool*>(
            safePeekMain(interpretter, {DcmBool::typeVal()}));
        interpretter->mainStack.pop();
        while (dcmBool->val) {
            dcmBool = static_cast<DcmBool*>(
                safePeekMain(interpretter, {DcmBool::typeVal()}));
            interpretter->mainStack.pop();
        }
        
        return NULL;
    }
} cbWhile;

Plugin *preludePlugin() {
    vector<NamedCB> v = 
        // Flow control
        { NamedCB("ex",     &cbX)
        , NamedCB("def",    new SimpleCallback(cbDef))
        , NamedCB("if",     new SimpleCallback(cbIf))
        , NamedCB("while",  new SimpleCallback(cbWhile))
        , NamedCB("until",  new SimpleCallback(cbWhile))
        , NamedCB("range",  new SimpleCallback(cbRange))
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
