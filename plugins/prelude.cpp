#include "prelude.h"

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
    Callback*

Plugin *preludePlugin() {
    vector<Callback*> v = 
        { addName(new SimpleCallback(cbDup), "dup")
        , addName(new SimpleCallback(cbDel), "del")
        , addName(new SimpleCallback(cbClear), "clear")
        , addName(&cbX, "x")
        };
    return new VectorPlugin(v);
}
