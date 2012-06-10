#include <stdexcept>
#include "prelude.h"

using namespace std;
using namespace Dcm;
using namespace Dcm::Prelude;

void cbConcat(DcmStack& stk) {
    DcmType **dcms = popN(stk, 2);
    bool mismatch = false;
    DcmType *ret=NULL;
    if (dcms[0]->isType(DcmExec::typeVal())) {
        if (dcms[1]->isType(DcmExec::typeVal())) {
            DcmExec *exec1 = (DcmExec*)dcms[1]->copy();
            DcmExec *exec0 = (DcmExec*)dcms[0];
            for (DcmType *dcm : (*exec0)) {
                dup(dcm);
            }
            exec1->insert(exec1->end(), exec0->begin(), exec0->end());
            ret = exec1;
        }
        else {
            mismatch = true;
        }
    }
    else if (dcms[0]->isType(DcmArray::typeVal())) {
        if (dcms[1]->isType(DcmArray::typeVal())) {
            DcmArray *arr0 = (DcmArray*)dcms[0];
            DcmArray *arr1 = (DcmArray*)dcms[1];
            DcmArray *arr  = new DcmArray(arr0->length() + arr1->length());
            int i;
            for (i=0; i < arr1->length(); i++) {
                del((*arr)[i]);
                (*arr)[i] = dup((*arr1)[i]);
            }
            for (;i < arr1->length() + arr0->length(); i++) {
                del((*arr)[i]);
                (*arr)[i] = dup((*arr0)[i-arr1->length()]);
            }
            ret = arr;
        }
        else {
            mismatch = true;
        }
    }
    else if (dcms[0]->isType(DcmString::typeVal())) { 
        if (dcms[1]->isType(DcmString::typeVal())) {
            ret = new DcmString((*(DcmString*)dcms[1]) + 
                (*(DcmString*)dcms[0]));
        }
        else {
            mismatch = true;
        }
    }
    if (ret) {
        del(dcms[0]);
        del(dcms[1]);
        delete[] dcms;
        stk.push(ret);
    }
    else {
        DcmTypeError *dcmTE;
        stk.push(dcms[1]);
        stk.push(dcms[0]);
        if (mismatch) {
            dcmTE = new DcmTypeError({dcms[0]->type()}, dcms[1]->type());
        }
        else {
            dcmTE = new DcmTypeError(
                { DcmExec::typeVal()
                , DcmArray::typeVal()
                , DcmString::typeVal()
                }, dcms[0]->type());
        }
        delete[] dcms;
        throw dcmTE;
    }
}

// It's because of complicated codepaths like this that we need SubText
class : public Callback {
    Callback *run(Interpretter *interpretter) {
        int choice=0;
        DcmTypeError *dcmTE = NULL;
        DcmType **dcms = popN(interpretter->mainStack, 2);
        if (dcms[0]->isType(DcmArray::typeVal())) {
            choice = 1;
        }
        else if (dcms[0]->isType(DcmString::typeVal())) {
            choice = 2;
        }
        else if (dcms[0]->isType(DcmNamespace::typeVal())) {
            choice = 3;
        }
        else {
            dcmTE = new DcmTypeError(
                { DcmArray::typeVal()
                , DcmString::typeVal()
                , DcmNamespace::typeVal()
                }, dcms[0]->type());
            choice = -1;
        }
        if (choice > 0 && !dcms[1]->isType(DcmExec::typeVal())) {
            dcmTE = new DcmTypeError({DcmExec::typeVal()}, dcms[1]->type());
            choice = -1;
        }
        switch(choice) {
          case 1:
            runArr(interpretter, dcms);
            break;
          case 2:
            runStr(interpretter, dcms);
            break;
          case 3:
            runNS(interpretter, dcms);
            break;
          case -1:
            interpretter->mainStack.push(dcms[1]);
            interpretter->mainStack.push(dcms[0]);
            delete[] dcms;
            throw dcmTE;
          default:
            throw new DcmError();
        }
        return NULL;
    }
    
    void runArr(Interpretter *interpretter, DcmType **dcms) {
        ExecRunner xBody(static_cast<DcmExec*>(dcms[1]));
        DcmArray *dcmArr = static_cast<DcmArray*>(dcms[0]);
        
        Finally finally([&]() {
            del(dcmArr);
            delete[] dcms;
        });
        
        for (int i = 0; i < dcmArr->length(); i++) {
            interpretter->mainStack.push(dup((*dcmArr)[i]));
            xBody.runExec(interpretter);
        }
    }
    
    void runStr(Interpretter *interpretter, DcmType **dcms) {
        ExecRunner xBody(static_cast<DcmExec*>(dcms[1]));
        DcmString *dcmStr = static_cast<DcmString*>(dcms[0]);
        
        Finally finally([&]() {
            del(dcmStr);
            delete[] dcms;
        });
        
        for (char ch : (*dcmStr)) {
            interpretter->mainStack.push(new DcmChar(ch));
            xBody.runExec(interpretter);
        }
    }
    
    void runNS(Interpretter *interpretter, DcmType **dcms) {
        ExecRunner xBody(static_cast<DcmExec*>(dcms[1]));
        interpretter->scope.push(dcms[0]->getNamespace());
        del(dcms[0]);

        Finally finally([&]() {
            del(interpretter->scope.top()->restore());
            interpretter->scope.pop();
            delete[] dcms;
        });
        
        for (auto item : (*interpretter->scope.top())) {
            if (!item.second.empty()) {
                interpretter->mainStack.push(new DcmSymbol(item.first));
                xBody.runExec(interpretter);
            }
        }
    }
} cbFor;

void cbLen(DcmStack& stk) {
    DcmType *dcm = safePeekMain(stk);
    stk.pop();
    if (dcm->isType(DcmString::typeVal())) {
        stk.push(new DcmInt(static_cast<DcmString*>(dcm)->length()));
    }
    else if (dcm->isType(DcmArray::typeVal())) {
        stk.push(new DcmInt(static_cast<DcmArray*>(dcm)->length()));
    }
    else {
        stk.push(dcm);
        throw new DcmTypeError(
            { DcmString::typeVal()
            , DcmArray::typeVal()
            }, dcm->type());
    }
    del(dcm);
}

void cbArray(DcmStack& stk) {
    DcmInt *dcmInt = static_cast<DcmInt*>(
        safePeekMain(stk, {DcmInt::typeVal()}));
    stk.pop();
    if (dcmInt->val >= 0) {
        stk.push(new DcmArray(dcmInt->val));
        del(dcmInt);
    }
    else {
        stk.push(dcmInt);
        throw new DcmBoundsError(0, dcmInt->val);
    }
}

// So not thread safe.
DcmStack delims;

void cbLParen(DcmStack& stk) {
    DcmType *delim = dup(new DcmNone());
    stk.push(delim);
    delims.push(delim);
}

void cbRParen(DcmStack& stk) {
    int count = 0;
    DcmStack elems;
    DcmType *delim, *cur;
    DcmArray *ret;

    if (delims.empty()) {
        throw new DcmStackError(new DcmSymbol("delim stack"), (TypeVal)"\0");
    }
    delim = delims.top();
    delims.pop();

    while (true) {
        if (stk.empty()) {
            while (!elems.empty()) {
                stk.push(elems.top());
                elems.pop();
            }
            del(delim);
            throw new DcmStackError(new DcmSymbol("main stack"), 
                (TypeVal)"\0");
        }
        cur = stk.top();
        stk.pop();
        if (cur == delim) {
            del(cur); del(delim);
            break;
        }
        else {
            count++;
            elems.push(cur);
        }
    }
    del(delim);
    
    ret = new DcmArray(count);
    for (int i=0; i < count; i++) {
        del((*ret)[i]);
        (*ret)[i] = elems.top();
        elems.pop();
    }

    stk.push(ret);
}

void cbAccess(DcmStack& stk) {
    DcmType **dcms = popN(stk, 2);
    if (!dcms[0]->isType(DcmInt::typeVal())) {
        stk.push(dcms[1]);
        stk.push(dcms[0]);
        DcmTypeError *dcmTE = new DcmTypeError({DcmInt::typeVal()},
            dcms[0]->type());
        delete[] dcms;
        throw dcmTE;
    }    
    DcmInt *dcmInt = static_cast<DcmInt*>(dcms[0]);
    
    if (dcms[1]->isType(DcmString::typeVal())) {
        DcmString *dcmStr = static_cast<DcmString*>(dcms[1]);
        
        try {
            stk.push(new DcmChar(dcmStr->at(dcmInt->val)));
        }
        catch (out_of_range ex) {
            stk.push(dcms[1]);
            stk.push(dcms[0]);
            DcmBoundsError *dcmBE = new DcmBoundsError(
                dcmStr->length() - 1, dcmInt->val);
            delete[] dcms;
            throw dcmBE;
        }
    }
    else if (dcms[1]->isType(DcmArray::typeVal())) {
        DcmArray *dcmArr = static_cast<DcmArray*>(dcms[1]);
        
        try {
            stk.push(dup((*dcmArr)[dcmInt->val]));
        }
        catch (DcmBoundsError *dcmBE) {
            stk.push(dcms[1]);
            stk.push(dcms[0]);
            delete[] dcms;
            throw dcmBE;
        }
    }
    else {
        stk.push(dcms[1]);
        stk.push(dcms[0]);
        DcmTypeError *dcmTE = new DcmTypeError(
            { DcmString::typeVal()
            , DcmArray::typeVal()
            }, dcms[1]->type());
        delete[] dcms;
        throw dcmTE;
    }
    del(dcms[0]);
    del(dcms[1]);
    delete[] dcms;
}

void cbAssign(DcmStack& stk) {
    DcmType **dcms = popN(stk, 3);
    if (!dcms[0]->isType(DcmInt::typeVal())) {
        stk.push(dcms[2]);
        stk.push(dcms[1]);
        stk.push(dcms[0]);
        DcmTypeError *dcmTE = new DcmTypeError({DcmInt::typeVal()},
            dcms[0]->type());
        delete[] dcms;
        throw dcmTE;
    }    
    DcmInt *dcmInt = static_cast<DcmInt*>(dcms[0]);
    
    if (dcms[2]->isType(DcmString::typeVal())) {
        if (!dcms[1]->isType(DcmChar::typeVal())) {
            stk.push(dcms[2]);
            stk.push(dcms[1]);
            stk.push(dcms[0]);
            DcmTypeError *dcmTE = new DcmTypeError({DcmChar::typeVal()},
                dcms[1]->type());
            delete[] dcms;
            throw dcmTE;
        }
        
        DcmString *dcmStr = static_cast<DcmString*>(dcms[2]);
        DcmChar *dcmCh = static_cast<DcmChar*>(dcms[1]);
        
        try {
            dcmStr->at(dcmInt->val) = dcmCh->val;
            del(dcmCh);
        }
        catch (out_of_range ex) {
            stk.push(dcms[2]);
            stk.push(dcms[1]);
            stk.push(dcms[0]);
            DcmBoundsError *dcmBE = new DcmBoundsError(
                dcmStr->length() - 1, dcmInt->val);
            delete[] dcms;
            throw dcmBE;
        }
    }
    else if (dcms[2]->isType(DcmArray::typeVal())) {
        DcmArray *dcmArr = static_cast<DcmArray*>(dcms[2]);
        
        try {
            del((*dcmArr)[dcmInt->val]);
            (*dcmArr)[dcmInt->val] = dcms[1];
        }
        catch (DcmBoundsError *dcmBE) {
            stk.push(dcms[2]);
            stk.push(dcms[1]);
            stk.push(dcms[0]);
            delete[] dcms;
            throw dcmBE;
        }
    }
    else {
        stk.push(dcms[2]);
        stk.push(dcms[1]);
        stk.push(dcms[0]);
        DcmTypeError *dcmTE = new DcmTypeError(
            { DcmString::typeVal()
            , DcmArray::typeVal()
            }, dcms[2]->type());
        delete[] dcms;
        throw dcmTE;
    }
    del(dcms[0]);
    stk.push(dcms[2]);
    delete[] dcms;
}

void groom(int& i, int len) {
    if (i < 0) {
        i += len;
        if (i < 0) i = 0;

    }
    if (i > len) i = len;
}

void cbSlice(DcmStack& stk) {
    DcmType **dcms = popN(stk, 2);
    if (!dcms[0]->isType(DcmInt::typeVal())) {
        stk.push(dcms[1]);
        stk.push(dcms[0]);
        DcmTypeError *dcmTE = new DcmTypeError({DcmInt::typeVal()},
            dcms[0]->type());
        delete[] dcms;
        throw dcmTE;
    }    
    int cut = static_cast<DcmInt*>(dcms[0])->val;

    if (dcms[1]->isType(DcmInt::typeVal())) {
        int start =  static_cast<DcmInt*>(dcms[1])->val;
        DcmType *dcm;
        try {
            dcm = safePeekMain(stk, 
                {DcmString::typeVal(), DcmArray::typeVal()});
            stk.pop();
        }
        catch (DcmError* ex) {
            stk.push(dcms[1]);
            stk.push(dcms[0]);
            delete[] dcms;
            throw ex;
        }
        
        if (cut < 0) cut = 0;
        
        // In these cases cut is the length
        if (dcm->isType(DcmString::typeVal())) {
            DcmString *dcmStr = static_cast<DcmString*>(dcm);
            groom(start, dcmStr->length());
            cut += start;
            groom(cut, dcmStr->length());
            
            DcmString *ret = new DcmString();
            ret->resize(cut - start);

            for (int i = start; i < cut; i++) {
            (*ret)[i-start] = (*dcmStr)[i];
            }
            stk.push(ret);
        }
        else /* if (dcm->isType(DcmArray::typeVal())) */ {
            DcmArray *dcmArr = static_cast<DcmArray*>(dcm);
            groom(start, dcmArr->length());
            cut += start;
            groom(cut, dcmArr->length());
            
            DcmArray *ret = new DcmArray(cut - start);

            for (int i=start; i < cut; i++) {
                del((*ret)[i-start]);
                (*ret)[i-start] = dup((*dcmArr)[i]);
            }
            stk.push(ret);
        }
    }
    else if (dcms[1]->isType(DcmString::typeVal())) {
        DcmString *dcmStr = static_cast<DcmString*>(dcms[1]);
        groom(cut, dcmStr->length());
        
        DcmString *ret = new DcmString();
        ret->resize(dcmStr->length() - cut);

        for (int i=cut; i < dcmStr->length(); i++) {
            (*ret)[i-cut] = (*dcmStr)[i];
        }
        stk.push(ret);
    }
    else if (dcms[1]->isType(DcmArray::typeVal())) {
        DcmArray *dcmArr = static_cast<DcmArray*>(dcms[1]);
        groom(cut, dcmArr->length());
        
        DcmArray *ret = new DcmArray(dcmArr->length() - cut);

        for (int i=cut; i < dcmArr->length(); i++) {
            del((*ret)[i-cut]);
            (*ret)[i-cut] = dup((*dcmArr)[i]);
        }
        stk.push(ret);
    }
    else {
        stk.push(dcms[1]);
        stk.push(dcms[0]);
        DcmTypeError *dcmTE = new DcmTypeError(
            { DcmInt::typeVal()
            , DcmString::typeVal()
            , DcmArray::typeVal()
            }, dcms[1]->type());
        delete[] dcms;
        throw dcmTE;
    }
    del(dcms[0]);
    del(dcms[1]);
    delete[] dcms;
}

void Dcm::Prelude::prelude_addSeq(vector<NamedCB>& vec) {
    vector<NamedCB> v = 
        { NamedCB("&",      new SimpleCallback(cbConcat))
        , NamedCB("(",      new SimpleCallback(cbLParen))
        , NamedCB(")",      new SimpleCallback(cbRParen))
        , NamedCB("(_)",    new SimpleCallback(cbAccess))
        , NamedCB("(=)",    new SimpleCallback(cbAssign))
        , NamedCB("(|)",    new SimpleCallback(cbSlice))
        , NamedCB("len",    new SimpleCallback(cbLen))
        , NamedCB("array",  new SimpleCallback(cbArray))
        , NamedCB("for",    &cbFor)
        };
    vec.insert(vec.end(), v.begin(), v.end());
}
