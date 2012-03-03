#include <iostream>
#include <fstream>
#include <utility>
#include "Interpretter.h"

#include <readline/readline.h>
#include <readline/history.h>

// Plugins
#include "plugins/io.h"
#include "plugins/prelude.h"
//#include "stdlib.h"

volatile bool done;

const char *Hist_File = ".history";

void cbfExit(DcmStack& stack) {
    done = true;
}

Plugin *mainPlugin() {
    vector<NamedCB> cbs =
        { NamedCB("quit",   new SimpleCallback(cbfExit))
        , NamedCB("exit",   new SimpleCallback(cbfExit))
        , NamedCB(":q",     new SimpleCallback(cbfExit))
        };
    return new VectorPlugin(cbs);
}

int findLineEnd(string& line, int start) {
    int end=start;
    bool inString = false;
    bool afterSlash = false;
    for (; end < line.size(); end++) {
        if (inString) {
            inString = !afterSlash && line[end] == '\"';
            if (line[end] == '\\') {
                afterSlash = !afterSlash;
            }
            else {
                afterSlash = false;
            }
        }
        else {
            if (line[end] == '\"') {
                inString = true;
            }
            else if (line[end] == '#') {
                break;
            }
        }
    }
    return end - 1;
}

string sanitize(char * cpLine) {
    string line(cpLine);
    int start, end;
    for (start=0; start < line.size(); start++)
        if (line[start] != ' ' && line[start] != '\t') break;
    end = findLineEnd(line, start);
    for (; end >= start; end--)
        if (line[end] != ' ' && line[end] != '\t') break;
    return line.substr(start, end - start + 1);
}

void interact(Interpretter& interpretter) {
    string histLine;
    char *line;
    unsigned int lineCount = 0;
    using_history();
    read_history(Hist_File);

    while (!done) {
        if (interpretter.isInString()) {
            line = readline("");
        }
        else if (interpretter.isInExec()) {
            line = readline("dcmi] ");
        }
        else {
            line = readline("dcmi> ");
        }
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
            if (interpretter.isInString()) {
                histLine.append(line);
                histLine.append("\\n");
            }
            else if (interpretter.isInExec()) {
                histLine.append(sanitize(line));
                histLine.append(" ");
            }
            else{
                histLine.append(sanitize(line));
                add_history(histLine.c_str());
                histLine = "";
                lineCount++;
            }
            delete line;
        }}
        else {
            cout << endl;
            break;
        }
    }
    
    append_history(lineCount, Hist_File);
    history_truncate_file(Hist_File, 100);
}

void runFile(Interpretter& interpretter, char* fn) {
    string line;
    fstream inFile;
    inFile.open(fn, fstream::in);
    if (!inFile) {
        cerr << "Could not open \"" << fn << "\"\n";
        return;
    }
    try {
        while (inFile) {
            getline(inFile, line);
            interpretter.execute(line);
        }
    }
    catch (DcmError *err) {
        cerr << err->repr() << endl;
    }
    catch (InterpretterError err) {
        cerr << err.what() << endl;
    }
    inFile.close();
}

int main(int argc, char **argv) {
    Interpretter interpretter;
    interpretter.addPlugin(*mainPlugin());
    interpretter.addPlugin(*ioPlugin());
    interpretter.addPlugin(*preludePlugin());
    
    if (argc > 2 && string(argv[1]) == "-f")
        runFile(interpretter, argv[2]);
    else
        interact(interpretter);
    return 0;
}
