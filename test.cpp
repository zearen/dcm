#include <iostream>
#include <utility>
#include "Interpretter.h"

// Plugins
#include "plugins/io.h"
#include "plugins/prelude.h"

volatile bool done;

void cbfExit(DcmStack& stack) {
    done = true;
}

Plugin *mainPlugin() {
    SimpleCallback *cbExit = new SimpleCallback(cbfExit);
    cbExit->name = "exit";
    vector<Callback*> cbs;
    cbs.push_back(cbExit);
    return new VectorPlugin(cbs);
}

int main() {
    Interpretter interpretter;
    string strLine;
    
    interpretter.addPlugin(*mainPlugin());
    interpretter.addPlugin(*ioPlugin());
    interpretter.addPlugin(*preludePlugin());
    
    while (!done) {
        getline(cin, strLine);
        try {
            interpretter.execute(strLine);
        }
        catch (DcmError *err) {
            cerr << err->repr() << endl;
        }
        catch (InterpretterError err) {
            cerr << err.what() << endl;
        }
    }
    return 0;
}
