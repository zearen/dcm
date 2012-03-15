#include "prelude.h"

void cbIs(DcmStack& stk) {
    DcmType **dcms = popN(stk, 2);
    stk.push(new DcmBool(dcms[0] == dcms[1]));
    del(dcms[0]); del(dcms[1]);
    delete dcms;
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

void cbIf(DcmStack& stk) {
    DcmType **dcms = popN(stk, 3);
    if (dcms[2]->isType(DcmBool::typeVal())) {
        if (static_cast<DcmBool*>(dcms[2])->val)
            stk.push(dup(dcms[1]));
        else
            stk.push(dup(dcms[0]));
    }
    else {
        DcmTypeError *dcmTE = new DcmTypeError(
            {DcmBool::typeVal()}, dcms[2]->type());
        stk.push(dcms[2]);
        stk.push(dcms[1]);
        stk.push(dcms[0]);
        delete dcms;
        throw dcmTE;
    }
    del(dcms[0]);
    del(dcms[1]);
    del(dcms[2]);
    delete dcms;
}

class : public Callback {
  public:
    Callback *run(Interpretter *interpretter) {
        DcmType **dcms = popN(interpretter->mainStack, 2);
        ExecRunner xBody, xCond;
        if (!dcms[0]->isType(DcmExec::typeVal())
          || !dcms[1]->isType(DcmExec::typeVal())) {
            DcmTypeError *dcmTE;
            if (!dcms[0]->isType(DcmExec::typeVal())) {
                dcmTE = new DcmTypeError( {DcmExec::typeVal()},
                    dcms[0]->type());
            }
            else {
                dcmTE = new DcmTypeError( {DcmExec::typeVal()},
                    dcms[1]->type());
            }
            interpretter->mainStack.push(dcms[1]);
            interpretter->mainStack.push(dcms[0]);
            delete dcms;
            throw dcmTE;
        }
        
        DcmBool *dcmBool;
        xBody.setExec(static_cast<DcmExec*>(dcms[0]));
        xCond.setExec(static_cast<DcmExec*>(dcms[1]));
        delete dcms;  // At this point, the exec garbage collection is
                      //+ handled by RAII

        xCond.runExec(interpretter);
        dcmBool = static_cast<DcmBool*>(
            safePeekMain(interpretter, {DcmBool::typeVal()}));
        interpretter->mainStack.pop();
        while (dcmBool->val) {
            xBody.runExec(interpretter);
            xCond.runExec(interpretter);
            del(dcmBool);
            dcmBool = static_cast<DcmBool*>(
                safePeekMain(interpretter, {DcmBool::typeVal()}));
            interpretter->mainStack.pop();
        }
        del(dcmBool);
        return NULL;
    }
} cbWhile;

class : public Callback {
    Callback *run(Interpretter *interpretter) {
        ExecRunner xBody(static_cast<DcmExec*>(
          safePeekMain(interpretter, {DcmExec::typeVal()})));
        interpretter->mainStack.pop();
        bool cont;
        DcmBool *dcmCont;
        do {
            xBody.runExec(interpretter);
            dcmCont = static_cast<DcmBool*>(
              safePeekMain(interpretter, {DcmBool::typeVal()}));
            interpretter->mainStack.pop();
            cont = dcmCont->val;
            del(dcmCont);
        } while(!cont);
    }
} cbUntil;

CharOp charRange(Interpretter *interpretter,
                ExecRunner& xBody,
                char step=1) {
    return [=, &xBody](char start, char end)->char {
        // Make sure the loop is iterating in the right direction
        if ((end - start) * step <= 0) {
            if (step < 0)
                throw new DcmBoundsError(start, end);
            else if (step > 0)
                throw new DcmBoundsError(end, start);
            else
                throw new DcmBoundsError(0, 0);
        }
        if (step > 0) {
            for (char i = start; i <= end; i += step) {
                interpretter->mainStack.push(new DcmChar(i));
                xBody.runExec(interpretter);
            }
        }
        else {
            for (char i = start; i >= end; i += step) {
                interpretter->mainStack.push(new DcmChar(i));
                xBody.runExec(interpretter);
            }
        }
        return 0;
    };
}

IntOp intRange(Interpretter *interpretter,
               ExecRunner& xBody,
               int step=1) {
    return [=, &xBody](int start, int end)->int {
        // Make sure the loop is iterating in the right direction
        if ((end - start) * step <= 0) {
            if (step < 0)
                throw new DcmBoundsError(start, end);
            else if (step > 0)
                throw new DcmBoundsError(end, start);
            else
                throw new DcmBoundsError(0, 0);
        }
        if (step > 0) {
            for (int i = start; i <= end; i += step) {
                interpretter->mainStack.push(new DcmInt(i));
                xBody.runExec(interpretter);
            }
        }
        else {
            for (int i = start; i >= end; i += step) {
                interpretter->mainStack.push(new DcmInt(i));
                xBody.runExec(interpretter);
            }
        }
        return 0;
    };
}

FloatOp floatRange(Interpretter *interpretter,
                 ExecRunner& xBody,
                 double step=1) {
    return [=, &xBody](double start, double end)->double {
        // Make sure the loop is iterating in the right direction
        if ((end - start) * step <= 0) {
            if (step < 0)
                throw new DcmBoundsError(start, end);
            else if (step > 0)
                throw new DcmBoundsError(end, start);
            else
                throw new DcmBoundsError(0, 0);
        }
        if (step > 0) {
            for (double i = start; i <= end; i += step) {
                interpretter->mainStack.push(new DcmFloat(i));
                xBody.runExec(interpretter);
            }
        }
        else {
            for (double i = start; i >= end; i += step) {
                interpretter->mainStack.push(new DcmFloat(i));
                xBody.runExec(interpretter);
            }
        }
        return 0;
    };
}

class : public Callback {
    Callback *run(Interpretter *interpretter) {
        DcmType **dcms = popN(interpretter->mainStack, 3);
        ExecRunner xBody;
        if (dcms[2]->isType(DcmExec::typeVal())) {
            xBody.setExec(static_cast<DcmExec*>(dcms[2]));
            try {
                doFloatOp(
                    floatRange(interpretter, xBody),
                    intRange(interpretter, xBody),
                    charRange(interpretter, xBody),
                    dcms[1], dcms[0]);
            }
            catch (DcmError *ex) {
                del(dcms[1]);
                del(dcms[0]);
                delete dcms;
                throw ex;
            }
            del(dcms[1]);
            del(dcms[0]);
            delete dcms;
        }
        else {
            double step;
            if (dcms[0]->isType(DcmChar::typeVal())) {
                step = (double)static_cast<DcmChar*>(dcms[0])->val;
            }
            else if (dcms[0]->isType(DcmInt::typeVal())) {
                step = (double)static_cast<DcmInt*>(dcms[0])->val;
            }
            else if (dcms[0]->isType(DcmFloat::typeVal())) {
                step = static_cast<DcmFloat*>(dcms[0])->val;
            }
            else {
                interpretter->mainStack.push(dcms[2]);
                interpretter->mainStack.push(dcms[1]);
                interpretter->mainStack.push(dcms[0]);
                delete dcms;
                throw new DcmTypeError(
                  { DcmChar::typeVal()
                  , DcmInt::typeVal()
                  , DcmFloat::typeVal()}, dcms[0]->type());
            }
            try {
                xBody.setExec(static_cast<DcmExec*>(
                  safePeekMain(interpretter, {DcmExec::typeVal()})));
                interpretter->mainStack.pop();
            }
            catch (DcmError *ex) {
                interpretter->mainStack.push(dcms[2]);
                interpretter->mainStack.push(dcms[1]);
                interpretter->mainStack.push(dcms[0]);
                delete dcms;
                throw ex;
            }
            try {

                doFloatOp(
                    floatRange(interpretter, xBody, step),
                    intRange(interpretter, xBody, (int)step),
                    charRange(interpretter, xBody, (char)step),
                    dcms[2], dcms[1]);
            }
            catch (DcmError *ex) {
                del(dcms[2]);
                del(dcms[1]);
                del(dcms[0]);
                delete dcms;
                throw ex;
            }
            del(dcms[2]);
            del(dcms[1]);
            del(dcms[0]);
            delete dcms;
        }
    }
} cbRange;

void prelude_addControl(vector<NamedCB>& vec) {
    vector<NamedCB> v = 
        { NamedCB("is",     new SimpleCallback(cbIs))
        , NamedCB("===",    new SimpleCallback(cbEquals))
        , NamedCB("=/=",    new SimpleCallback(cbNotEquals))
        , NamedCB("if",     new SimpleCallback(cbIf))
        , NamedCB("while",  &cbWhile)
        , NamedCB("until",  &cbUntil)
        , NamedCB("range",  &cbRange)
        };
    vec.insert(vec.end(), v.begin(), v.end());
}
