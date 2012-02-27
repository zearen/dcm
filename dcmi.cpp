#include <iostream>
#include <utility>
#include "Interpretter.h"

#include <readline/readline.h>
#include <readline/history.h>

// Plugins
#include "plugins/io.h"
#include "plugins/prelude.h"
//#include "stdlib.h"

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
    char *line;
    unsigned int lineCount = 0;
    using_history();
    read_history("~/.dcmi/history");

    Interpretter interpretter;
    interpretter.addPlugin(*mainPlugin());
    interpretter.addPlugin(*ioPlugin());
    interpretter.addPlugin(*preludePlugin());
    
    while (!done) {
        line = readline("dcmi> ");
        if (line) { if (*line) {
            try {
                interpretter.execute(string(line));
            }
            catch (DcmError *err) {
                cerr << err->repr() << endl;
            }
            catch (InterpretterError err) {
                cerr << err.what() << endl;
            }
            add_history(line);
            delete line;
            lineCount++;
        }}
        else break;
    }

    append_history(lineCount, "~/.dcmi/history");
    history_truncate_file("~/dcmi/history", 100);
    return 0;
}
