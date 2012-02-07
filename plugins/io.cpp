#include "io.h"
#include <iostream>

using namespace std;

void cbPrint(DcmStack& stk) {
    cout << safePeekMain(stk)->repr() << endl;
}

void cbF(DcmStack& stk) {
    stack<DcmType*> items;
    while (!stk.empty()) {
        items.push(stk.top());
        stk.pop();
    }
    while (!items.empty()) {
        cout << items.top()->repr() << ' ';
        stk.push(items.top());
        items.pop();
    }
    cout << endl;
}


Plugin * ioPlugin() {
    vector<Callback*> v =
        { addName(new SimpleCallback(cbPrint), "print")
        , addName(new SimpleCallback(cbF), "f")
        };
    return new VectorPlugin(v);
}
