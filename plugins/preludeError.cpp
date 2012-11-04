#include "prelude.h"

using namespace std;
using namespace Dcm;
using namespace Dcm::Prelude;



void Dcm::Prelude::prelude_addError(vector<NamedCB>& vec) {
    vector<NamedCB> v =
        { NamedCB("class",      new SimpleCallback(cbClass))
        , NamedCB("namespace",  new SimpleCallback(cbNamespace))
        , NamedCB(":",          new SimpleCallback(cbNamespace))
        , NamedCB("{",          &cbLBrace)
        , NamedCB("}",          &cbRBrace)
        , NamedCB("inherit",    new SimpleCallback(cbInherit))
        // Let's depreciate this in favor of >:
        , NamedCB("obj",        new SimpleCallback(cbInherit))
        , NamedCB(">:",         new SimpleCallback(cbInherit))
        , NamedCB("_",          &cbLookback)
        , NamedCB("upcopy",     &cbUpcopy)
        , NamedCB("me",         &cbMe)
        , NamedCB(";",          &cbConstruct)
        };
    vec.insert(vec.end(), v.begin(), v.end());
}
