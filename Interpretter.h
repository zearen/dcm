/*
    Zachary Weaver
    23/07/2011
    Interpretter.h
*/

#include <string>
#include <vector>
#include <stack>

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
    private:
        // Sub-parsers
        void peek(string stkName);
        void pop(string stkName);
        void push(string stkName);
        void swap(string stkName);
        void empty(string stkName);
        void attrib(string attr);
        void literal(string lit);
    protected:
        // This namespace is search first
        unordered_map<string, cb*> heaven;
        // Then we decend through scope
        stack<Namespace> scope;
        DcmStack mainStack;
        // If an exec wraps a line
        stack<DcmExec*> cont;
        // If a string wraps a line
        string strCont;
    public:
        Interpretter();
        Interpretter(vector<Plugin>);
        ~Interpretter();
        
        void addPlugin(Plugin);
        
        void execute(string commands);
        
        static DcmExec *Parse(string str);
};
