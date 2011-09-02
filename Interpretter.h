/*
    Zachary Weaver
    23/07/2011
    Interpretter.h
*/

#ifndef _INTERPRETTER_H
#define _INTERPRETTER_H

#include <string>
#include <vector>

#include "DcmType/DcmType.h"
//#include "Plugin.h"

using namespace std;
class InterpretterError : public exception {
    private:
        string msg;
    public:
        InterpretterError();
        InterpretterError(string& message);
        ~InterpretterError() throw ();
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
//        vector<Plugin>
    protected:
        // If an exec wraps a line
        stack<DcmExec*> cont;
        // If a string wraps a line
        string strCont;
    public:
        // This namespace is search first
        unordered_map<string, Callback*> heaven;
        // Then we decend through scope
        stack<Namespace*> scope;
        DcmStack mainStack;
        
        Interpretter();
//        Interpretter(vector<Plugin>);
        ~Interpretter();
        
//        void addPlugin(Plugin);
        
        void execute(string commands);
        
        static DcmExec *Parse(string str);
};
#endif
