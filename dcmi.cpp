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
    vector<NamedCB> cbs =
        { NamedCB("quit",   new SimpleCallback(cbfExit))
        , NamedCB("exit",   new SimpleCallback(cbfExit))
        , NamedCB("q",      new SimpleCallback(cbfExit))
        , NamedCB(":q",     new SimpleCallback(cbfExit))
        };
    return new VectorPlugin(cbs);
}

int main() {
    Interpretter interpretter;
    string strLine;
    
    interpretter.addPlugin(*mainPlugin());
    interpretter.addPlugin(*ioPlugin());
    interpretter.addPlugin(*preludePlugin());
    
    while (!done) {
        cout << "> ";
        cout.flush();
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
