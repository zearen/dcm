// Test 1 for the library.  We just have a library for printing

#include <iostream>
#include "Interpretter.h"

bool done;

class : public Callback {
    Callback *run(Interpretter *interpretter) {
        if (!interpretter->mainStack.empty()) {
            cout << interpretter->mainStack.top()->repr() << endl;
        }
    }
} cbPrint;

void cbfExit(DcmStack& stack) {
    done = true;
}

Plugin mainPlugin() {
    SimpleCallback cbExit(cbfExit);
    cbExit.name = "exit";
    cbPrint.name = "print";
    return VectorPlugin( vector<Callback*> {
        &cbPrint,
        &cbExit
    } );
}

int main() {
    Interpretter interpretter;
    string strLine;
    
    while (!done) {
        getline(cin, strLine);
        interpretter.execute(strLine);
    }
    return 0;
}
