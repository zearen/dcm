#include "prelude.h"
#include "../ExecParser.h"
#include <stdexcept>

DcmClass *dcmClass = new DcmClass();

void cbClass(DcmStack& stk) {
    stk.push(dup(dcmClass));
}

void cbNamespace(DcmStack& stk) {
    stk.push(new DcmNamespace());
}

class : public Callback {
  Callback *run(Interpretter *interpretter) {
    DcmType *dcm = safePeekMain(interpretter);
    Namespace *ns = dcm->getNamespace();
    if (ns) {
        interpretter->scope.push(ns);
        interpretter->mainStack.pop();
        del(dcm);
    }
    else {
        throw new DcmTypeError(
            { DcmNamespace::typeVal()
            , DcmClass::typeVal()}, dcm->type());
    }
    return NULL;
  }
} cbLBrace;

class : public Callback {
  Callback *run (Interpretter *interpretter) {
    DcmType *dcm = interpretter->scope.top()->restore();
    if (dcm) {
        interpretter->scope.pop();
        interpretter->mainStack.push(dcm);
    }
    else {
        throw new DcmStackError(new DcmSymbol("scope stack"),
            DcmNone::typeVal());
    }
    return NULL;
  }
} cbRBrace;

void cbInherit(DcmStack& stk) {
    DcmClass *dcm = static_cast<DcmClass*>(
        safePeekMain(stk, {DcmClass::typeVal()}));
    stk.pop();
    stk.push(new DcmClass(dcm));
    del(dcm);
}

class : public Callback {
  Callback *run(Interpretter *interpretter) {
    Namespace *top = interpretter->scope.top();
    DcmType *dcm;
    if (top->id() == 'c') {
        dcm = static_cast<DcmClass*>(top)->base();
    }
    else {
        interpretter->scope.pop();
        Namespace *back = interpretter->scope.top();
        interpretter->scope.push(top);
        dcm = back->restore();
    }
    if (dcm) {
        interpretter->mainStack.push(dup(dcm));
    }
    else {
        throw new DcmStackError(new DcmSymbol("scope stack"),
            DcmNone::typeVal());
    }
    return NULL;
  }
} cbLookback;

class : public Callback {
  Callback *run(Interpretter *interpretter) {
    DcmSymbol *dcm = static_cast<DcmSymbol*>(
        safePeekMain(interpretter, {DcmSymbol::typeVal()}));
    interpretter->mainStack.pop();
    string sym = dcm->get();
    bool needFetch = false;
    try {
        // If we already have it, we don't need to copy it.
        needFetch = interpretter->scope.top()->at(sym).empty();
    }
    catch (out_of_range ex) {
        needFetch = true;
    }
    if (needFetch) {
        DcmType *upcopy = raw_peek(sym, &interpretter->scope);
        if (upcopy) {
            (*interpretter->scope.top())[sym].push(dup(upcopy));
        }
        else {
            interpretter->mainStack.push(dcm);
            throw new DcmStackError(static_cast<DcmSymbol*>(dup(dcm)),
                DcmSymbol::typeVal());
        }
    }
    del(dcm);
    return NULL;
  }
} cbUpcopy;

class : public Callback {
  Callback *run(Interpretter *interpretter) {
    DcmType *dcm = interpretter->scope.top()->restore();
    if (dcm) {
        interpretter->mainStack.push(dup(dcm));
    }
    else {
        throw new DcmStackError(new DcmSymbol("scope stack"),
            DcmNone::typeVal());
    }
    return NULL;
  }
} cbMe;

static string semiStr = ";";
static Callback *semiAttrib = new AttribCallback(semiStr);
class : public Callback {
  Callback *run(Interpretter *interpretter) {
        cbInherit(interpretter->mainStack);
        semiAttrib->run(interpretter);
        return NULL;
  }
} cbConstruct;

void prelude_addOOP(vector<NamedCB>& vec) {
    vector<NamedCB> v = 
        { NamedCB("class",      new SimpleCallback(cbClass))
        , NamedCB("namespace",  new SimpleCallback(cbNamespace))
        , NamedCB(":",          new SimpleCallback(cbNamespace))
        , NamedCB("{",          &cbLBrace)
        , NamedCB("}",          &cbRBrace)
        , NamedCB("inherit",    new SimpleCallback(cbInherit))
        , NamedCB("obj",        new SimpleCallback(cbInherit))
        , NamedCB("_",          &cbLookback)
        , NamedCB("upcopy",     &cbUpcopy)
        , NamedCB("me",         &cbMe)
        , NamedCB(";",          &cbConstruct)
        };
    vec.insert(vec.end(), v.begin(), v.end());
}

