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
#include "Plugin.h"

using namespace std;
class InterpretterError : public exception {
    private:
        string msg;
    public:
        InterpretterError();
        InterpretterError(string message);
        ~InterpretterError() throw ();
        const char* what();
};

class Interpretter {
    protected:
        // If an exec wraps a line
        stack<DcmExec*> cont;
        // If a string wraps a line
        string strCont;
    public:
        // This namespace is search first
        Namespace heaven;
        // Then we decend through scope
        Scope scope;
        DcmStack mainStack;
        
        Interpretter();
        Interpretter(vector<Plugin> plugins);
        ~Interpretter();
        
        // Link the plugin into heaven
        void addPlugin(Plugin& plugin);
        
        // Execute given command
        // That the input stream well accept anything but \n
        void execute(string commands) throw (DcmError*);
        
        // Returns whether the parser is receiving a multiline string
        bool isInString();

        // Returns whether the parser is receiving a multiline exec
        bool isInExec();
        
//        static DcmExec *Parse(string& str);
        
    private:
        // Sub-parsers
        void peek(string& stkName, int& i, bool checkHeaven);
        void pop(string& stkName, int& i);
        void push(string& stkName, int& i);
        void swap(string& stkName, int& i);
        void empty(string& stkName, int& i);
        void attrib(string& attr, int& i);
        void exec(string& execStr, int& i);
        static DcmSymbol *sym(string& symName, int& i);
        void str(string& strng, int& i);
        static DcmChar *ch(string& c, int& i);
        static DcmElem *number(string& num, int& i);
        
        // Constructors for the exec parser
        void ex_push(string& commands, int& i);
        void ex_pop(string& commands, int& i);
        void ex_swap(string& commands, int& i);
        void ex_empty(string& commands, int& i);
        void ex_peek(string& commands, int& i, bool checkScope);
        void ex_attrib(string& commands, int& i);
        
    friend class PeekCallback;
    friend class PopCallback;
    friend class PushCallback;
    friend class SwapCallback;
    friend class EmptyCallback;
    friend class AttribCallback;
};

int isEndChar(char c);

int findEnd (string& str, int& i);

void skipWhitespace(string& str, int& i);
#endif
