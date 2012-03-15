#include "prelude.h"

typedef function<void(DcmStack&)> StackProc;

typedef function<bool(bool, bool)> BoolOp;
typedef function<bool(double, double)> NumComp;
typedef function<bool(string&, string&)> StrComp;


DcmNum * doIntOp(IntOp intOp,
                 CharOp charOp,
                 DcmType *left,
                 DcmType *right) 
                 throw (DcmTypeError*) {
    if (right->isType(DcmInt::typeVal())) {
        if (left->isType(DcmInt::typeVal())) {
            return new DcmInt(intOp(
                static_cast<DcmInt*>(left)->val,
                static_cast<DcmInt*>(right)->val
            ));
        }
        else if (left->isType(DcmChar::typeVal())) {
            return new DcmChar((char)intOp(
                (int)static_cast<DcmChar*>(left)->val,
                static_cast<DcmInt*>(right)->val
                ));
        }
        throw new DcmTypeError({DcmInt::typeVal(),
                                DcmChar::typeVal()}, left->type());
    }
    else if (left->isType(DcmInt::typeVal())) {
        if (right->isType(DcmChar::typeVal())) {
            return new DcmChar((char)intOp(
                static_cast<DcmInt*>(left)->val,
                (int)static_cast<DcmChar*>(right)->val
                ));
        }
        else throw new DcmTypeError({DcmInt::typeVal(),
                                    DcmChar::typeVal()}, right->type());
    }
    else if (right->isType(DcmChar::typeVal())) {
        if (left->isType(DcmChar::typeVal())) {
            return new DcmChar(charOp(
                static_cast<DcmChar*>(left)->val,
                static_cast<DcmChar*>(right)->val
                ));
        }
        else throw new DcmTypeError({DcmInt::typeVal(),
                                    DcmChar::typeVal()}, left->type());
    }
    else throw new DcmTypeError({DcmInt::typeVal(),
                            DcmChar::typeVal()}, right->type());
}

DcmNum * doFloatOp(FloatOp floatOp,
                   IntOp intOp,
                   CharOp charOp,
                   DcmType *left,
                   DcmType *right) 
                   throw (DcmTypeError*) {
    if (right->isType(DcmFloat::typeVal())) {
        if (left->isType(DcmFloat::typeVal())) {
            return new DcmFloat(floatOp(
                static_cast<DcmFloat*>(left)->val,
                static_cast<DcmFloat*>(right)->val
                ));
        }
        else if (left->isType(DcmInt::typeVal())) {
            return new DcmFloat(floatOp(
                (double)static_cast<DcmInt*>(left)->val,
                static_cast<DcmFloat*>(right)->val
                ));
        }
        else if (left->isType(DcmChar::typeVal())) {
            return new DcmFloat(floatOp(
                (double)static_cast<DcmChar*>(left)->val,
                static_cast<DcmFloat*>(right)->val
                ));
        }
        else throw new DcmTypeError({DcmFloat::typeVal(),
                                    DcmInt::typeVal(),
                                    DcmChar::typeVal()}, left->type());
    }
    else if (left->isType(DcmFloat::typeVal())) {
        if (right->isType(DcmInt::typeVal())) {
            return new DcmFloat(floatOp(
                static_cast<DcmFloat*>(left)->val,
                (double)static_cast<DcmInt*>(right)->val
                ));
        }
        else if (right->isType(DcmChar::typeVal())) {
            return new DcmFloat(floatOp(
                static_cast<DcmFloat*>(left)->val,
                (double)static_cast<DcmChar*>(right)->val
                ));
        }
        else throw new DcmTypeError({DcmFloat::typeVal(),
                                    DcmInt::typeVal(),
                                    DcmChar::typeVal()}, right->type());
    }
    // Not a double, so check ints
    else {
        try {
            return doIntOp(intOp, charOp, left, right);
        }
        catch (DcmTypeError *ex) {
            DcmTypeError *dcmTE = new DcmTypeError(
                {DcmFloat::typeVal(), DcmInt::typeVal(), DcmChar::typeVal()},
                ex->received());
            del(ex);
            throw dcmTE;
        }
    }
}

StackProc stackFloatOp(FloatOp floatOp, 
                       IntOp intOp,
                       CharOp charOp) {
    return [&](DcmStack& stk)->void {
        DcmType **dcms = popN(stk, 2);
        try {
            DcmType *newNum = doFloatOp(
                floatOp, intOp, charOp, dcms[1], dcms[0]);
            stk.push(newNum);
        }
        catch (DcmTypeError *ex) {
            stk.push(dcms[1]);
            stk.push(dcms[0]);
            delete dcms;
            throw ex;
        }
        del(dcms[0]);
        del(dcms[1]);
        delete dcms;
    };
}

StackProc stackIntOp(IntOp intOp,
                     CharOp charOp) {
   return [&](DcmStack& stk) {
        DcmType **dcms = popN(stk, 2);
        try {
            DcmType *newNum = doIntOp(
                intOp, charOp, dcms[1], dcms[0]);
            stk.push(newNum);
        }
        catch (DcmTypeError *ex) {
            stk.push(dcms[1]);
            stk.push(dcms[0]);
            delete dcms;
            throw ex;
        }
        del(dcms[0]);
        del(dcms[1]);
        delete dcms;
    };
}

StackProc stackBoolOp(BoolOp boolOp,
                      IntOp intOp,
                      CharOp charOp) {
    return [&](DcmStack& stk) {
        DcmType **dcms = popN(stk, 2);
        if (dcms[0]->isType(DcmBool::typeVal())
            && dcms[1]->isType(DcmBool::typeVal())) {
            stk.push(new DcmBool(boolOp(
                static_cast<DcmBool*>(dcms[1])->val,
                static_cast<DcmBool*>(dcms[0])->val
                )));
        }
        else try {
            DcmType *newNum = doIntOp(
                intOp, charOp, dcms[1], dcms[0]);
            stk.push(newNum);
        }
        catch (DcmTypeError *ex) {
            DcmTypeError *dcmTE = new DcmTypeError(
                { DcmBool::typeVal()
                , DcmInt::typeVal()
                , DcmChar::typeVal()
                }, ex->received());
            del(ex);
            stk.push(dcms[1]);
            stk.push(dcms[0]);
            delete dcms;
            throw dcmTE;
        }
        del(dcms[0]);
        del(dcms[1]);
        delete dcms;
    };
}

double cast2Double(DcmType* dcm) {
    if (dcm->isType(DcmFloat::typeVal())) {
        return static_cast<DcmFloat*>(dcm)->val;
    }
    else if (dcm->isType(DcmInt::typeVal())) {
        return (double)static_cast<DcmInt*>(dcm)->val;
    }
    else if (dcm->isType(DcmChar::typeVal())) {
        return (double)static_cast<DcmChar*>(dcm)->val;
    }
    else throw new DcmTypeError(
        // We're only ever going to call this from stackCompOp
        // So we'll just throw in strings to save time
        { DcmString::typeVal()
        , DcmFloat::typeVal()
        , DcmInt::typeVal()
        , DcmChar::typeVal()
        }, dcm->type());
}

StackProc stackCompOp(StrComp strComp, NumComp numComp) {
    return [&](DcmStack& stk) {
        DcmType **dcms = popN(stk, 2);
        if (dcms[0]->isType(DcmString::typeVal())
            && dcms[1]->isType(DcmString::typeVal())) {
            stk.push(new DcmBool(strComp(
                *static_cast<DcmString*>(dcms[1]),
                *static_cast<DcmString*>(dcms[0])
                )));
        }
        else try {
            double left = cast2Double(dcms[1]);
            double right = cast2Double(dcms[0]);
            stk.push(new DcmBool(numComp(left, right)));
        }
        catch (DcmTypeError *ex) {
            stk.push(dcms[1]);
            stk.push(dcms[0]);
            delete dcms;
            throw ex;
        }
        del(dcms[0]);
        del(dcms[1]);
        delete dcms;
    };
}

void cbAdd(DcmStack& stk) {
    stackFloatOp(
    [](double x, double y)->double{return x+y;},
    [](int x, int y)->int         {return x+y;},
    [](char x, char y)->char      {return x+y;})(stk);
}

void cbSub(DcmStack& stk) {
    stackFloatOp(
    [](double x, double y)->double{return x-y;},
    [](int x, int y)->int         {return x-y;},
    [](char x, char y)->char      {return x-y;})(stk);
}

void cbMul(DcmStack& stk) {
    stackFloatOp(
    [](double x, double y)->double{return x*y;},
    [](int x, int y)->int         {return x*y;},
    [](char x, char y)->char      {return x*y;})(stk);
}

void cbDiv(DcmStack& stk) {
    stackFloatOp(
    [](double x, double y)->double{return x/y;},
    [](int x, int y)->int         {return x/y;},
    [](char x, char y)->char      {return x/y;})(stk);
}


void cbMod(DcmStack& stk) {
    stackIntOp(
    [](int x, int y)->int   {return x%y;},
    [](char x, char y)->char{return x%y;})(stk);
}

void cbRShift(DcmStack& stk) {
    stackIntOp(
    [](int x, int y)->int   {return x>>y;},
    [](char x, char y)->char{return x>>y;})(stk);
}

void cbLShift(DcmStack& stk) {
    stackIntOp(
    [](int x, int y)->int   {return x<<y;},
    [](char x, char y)->char{return x<<y;})(stk);
}

void cbOr(DcmStack& stk) {
    stackBoolOp(
    [](bool x, bool y)->bool{return x||y;},
    [](int x, int y)->int   {return x|y;},
    [](char x, char y)->char{return x|y;})(stk);
}

void cbAnd(DcmStack& stk) {
    stackBoolOp(
    [](bool x, bool y)->bool{return x&&y;},
    [](int x, int y)->int   {return x&y;},
    [](char x, char y)->char{return x&y;})(stk);
}


void cbXor(DcmStack& stk) {
    stackBoolOp(
    [](bool x, bool y)->bool{return x!=y;},
    [](int x, int y)->int   {return x^y;},
    [](char x, char y)->char{return x^y;})(stk);
}


void cbNot(DcmStack& stk) {
    DcmType *dcm = safePeekMain(stk);
    stk.pop();
    if (dcm->isType(DcmBool::typeVal())) {
        stk.push(new DcmBool(!static_cast<DcmBool*>(dcm)->val));
    }
    else if (dcm->isType(DcmInt::typeVal())) {
        stk.push(new DcmInt(~static_cast<DcmInt*>(dcm)->val));
    }
    else if (dcm->isType(DcmChar::typeVal())) {
        stk.push(new DcmChar(~static_cast<DcmChar*>(dcm)->val));
    }
    else {
        del(dcm);
        throw new DcmTypeError(
            { DcmBool::typeVal()
            , DcmInt::typeVal()
            , DcmChar::typeVal()
            }, dcm->type());
    }
    del(dcm);
}
void cbLT(DcmStack& stk) {
    stackCompOp(
    [](string& x, string& y)->bool  {return x<y;},
    [](double x, double y)->bool    {return x<y;})(stk);
}

void cbGT(DcmStack& stk) {
    stackCompOp(
    [](string& x, string& y)->bool  {return x>y;},
    [](double x, double y)->bool    {return x>y;})(stk);
}

void cbLTE(DcmStack& stk) {
    stackCompOp(
    [](string& x, string& y)->bool  {return x<=y;},
    [](double x, double y)->bool    {return x<=y;})(stk);
}

void cbGTE(DcmStack& stk) {
    stackCompOp(
    [](string& x, string& y)->bool  {return x>=y;},
    [](double x, double y)->bool    {return x>=y;})(stk);
}

void cbEqual(DcmStack& stk) {
    stackCompOp(
    [](string& x, string& y)->bool  {return x==y;},
    [](double x, double y)->bool    {return x==y;})(stk);
}

void cbNotEqual(DcmStack& stk) {
    stackCompOp(
    [](string& x, string& y)->bool  {return x!=y;},
    [](double x, double y)->bool    {return x!=y;})(stk);
}

void prelude_addMath(vector<NamedCB>& vec) {
    vector<NamedCB> v = 
        // Comparison
        { NamedCB("<",      new SimpleCallback(cbLT))
        , NamedCB(">",      new SimpleCallback(cbGT))
        , NamedCB("<=",     new SimpleCallback(cbLTE))
        , NamedCB(">=",     new SimpleCallback(cbGTE))
        , NamedCB("==",     new SimpleCallback(cbEqual))
        , NamedCB("/=",     new SimpleCallback(cbNotEqual))
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
    vec.insert(vec.end(), v.begin(), v.end());
}

