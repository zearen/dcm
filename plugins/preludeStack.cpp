#include <stdexcept>

#include "prelude.h"
#include "../ExecParser.h"

using namespace std;
using namespace Dcm;
using namespace Dcm::Prelude;

class : public Callback {
  Callback *run(Interpretter *interpretter) {
    DcmSymbol *dcm = static_cast<DcmSymbol*>(
        safePeekMain(interpretter, {DcmSymbol::typeVal()}));
    interpretter->mainStack.pop();
    string sym = dcm->get();
    del(dcm);
    PopCallback cb(sym);
    cb.run(interpretter);
    return NULL;
  }
} cbPop;

class : public Callback {
  Callback *run(Interpretter *interpretter) {
    DcmSymbol *dcm = static_cast<DcmSymbol*>(
        safePeekMain(interpretter, {DcmSymbol::typeVal()}));
    interpretter->mainStack.pop();
    string sym = dcm->get();
    del(dcm);
    PushCallback cb(sym);
    cb.run(interpretter);
    return NULL;
  }
} cbPush;

class : public Callback {
  Callback *run(Interpretter *interpretter) {
    DcmSymbol *dcm = static_cast<DcmSymbol*>(
        safePeekMain(interpretter, {DcmSymbol::typeVal()}));
    interpretter->mainStack.pop();
    string sym = dcm->get();
    del(dcm);
    PeekCallback cb(sym, true);
    cb.run(interpretter);
    return NULL;
  }
} cbPeek;

class : public Callback {
  Callback *run(Interpretter *interpretter) {
    DcmSymbol *dcm = static_cast<DcmSymbol*>(
        safePeekMain(interpretter, {DcmSymbol::typeVal()}));
    interpretter->mainStack.pop();
    string sym = dcm->get();
    del(dcm);
    SwapCallback cb(sym);
    cb.run(interpretter);
    return NULL;
  }
} cbSwap;

class : public Callback {
  Callback *run(Interpretter *interpretter) {
    DcmSymbol *dcm = static_cast<DcmSymbol*>(
        safePeekMain(interpretter, {DcmSymbol::typeVal()}));
    interpretter->mainStack.pop();
    string sym = dcm->get();
    del(dcm);
    EmptyCallback cb(sym);
    cb.run(interpretter);
    return NULL;
  }
} cbEmpty;

class : public Callback {
  Callback *run(Interpretter *interpretter) {
    DcmSymbol *dcmSym = static_cast<DcmSymbol*>(
        safePeekMain(interpretter, {DcmSymbol::typeVal()}));
    interpretter->mainStack.pop();
    string sym = dcmSym->get();
    del(dcmSym);
    DcmType *dcm = safePeekMain(interpretter);
    interpretter->mainStack.pop();
    try {
        DcmStack &stk = interpretter->scope.top()->at(sym);
        if (!stk.empty()) {
            del(stk.top());
            stk.pop();
        }
        stk.push(dcm);
    }
    catch (out_of_range ex) {
        (*interpretter->scope.top())[sym].push(dcm);
    }
    return NULL;
  }
} cbSet;

class : public Callback {
  Callback *run(Interpretter *interpretter) {
    DcmSymbol *dcmSym = static_cast<DcmSymbol*>(
        safePeekMain(interpretter, {DcmSymbol::typeVal()}));
    interpretter->mainStack.pop();
    string sym = dcmSym->get();
    del(dcmSym);
    ExecRunner xAct(static_cast<DcmExec*>(
        safePeekMain(interpretter, {DcmExec::typeVal()})));
    interpretter->mainStack.pop();
    SwapCallback cbSwp(sym);
    cbSwp.run(interpretter);
    
    Finally finally( [&](){cbSwp.run(interpretter);});
            
    xAct.runExec(interpretter);
    return NULL;
  }
} cbWith;

void Dcm::Prelude::prelude_addStack(vector<NamedCB>& vec) {
    vector<NamedCB> v = 
        { NamedCB("=",      &cbSet)
        , NamedCB("pop",    &cbPop)
        , NamedCB("push",   &cbPush)
        , NamedCB("peek",   &cbPeek)
        , NamedCB("swap",   &cbSwap)
        , NamedCB("empty",  &cbEmpty)
        , NamedCB("with",   &cbWith)
        };
    vec.insert(vec.end(), v.begin(), v.end());
}
