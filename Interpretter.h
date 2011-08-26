/*
    Zachary Weaver
    23/07/2011
    Interpretter.h
*/

#include <string>
#include <vector>

#include "Type/DcmType.h"
#include "Plugin.h"

using namespace std;
using namespace DcmType;

class InterpretterError : public exception {
    private:
        string msg;
    public:
        InterpretterError();
        InterpretterError(string& message);
        const char* what();
};

class Interpretter {
    protected:
        stack<Namespace, slist> scope;
        DcmStack mainStack;
        // If an exec wraps a line
        stack<vector<DcmType*>, slist> cont;
        // If a string wraps a line
        string strCont;
    public:
        Interpretter();
        Interpretter(vector<Plugin>);
        ~Interpretter();
        
        void execute(string commands);
        void execute(DcmExec commands);
        void addPlugin(Plugin);
        
        static DcmExec Parse(string str);
    private:
        // We may not need these
        DcmType parse(string item);
        vector<string>tokenize(string line)
};
