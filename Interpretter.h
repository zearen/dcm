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

namespace Dcm {
class InterpretterError : public std::exception {
    private:
	std::string msg;
    public:
        InterpretterError();
        InterpretterError(std::string message);
        ~InterpretterError() throw ();
        const char* what();
};

class Interpretter {
    protected:
        // If an exec wraps a line
		std::stack<DcmExec*> cont;
        // If a string wraps a line
		std::string strCont;
    public:
        // This namespace is search first
        Namespace heaven;
        // Then we decend through scope
        Scope scope;
        DcmStack mainStack;
        
        Interpretter();
        Interpretter(std::vector<Plugin> plugins);
        ~Interpretter();
        
        // Link the plugin into heaven
        void addPlugin(Plugin& plugin);
        
        // Execute given command
        // That the input stream well accept anything but \n
        void execute(std::string commands) throw (DcmError*);
        
        // Returns whether the parser is receiving a multiline string
        bool isInString();

        // Returns whether the parser is receiving a multiline exec
        bool isInExec();
        
//        static DcmExec *Parse(string& str);
        
    private:
        // Sub-parsers
        void peek(std::string& stkName, int& i,
                  bool checkHeaven, bool runPrimFun=true);
        void pop(std::string& stkName, int& i);
        void push(std::string& stkName, int& i);
        void swap(std::string& stkName, int& i);
        void empty(std::string& stkName, int& i);
        void attrib(std::string& attr, int& i);
        void exec(std::string& execStr, int& i);
        static DcmSymbol *sym(std::string& symName, int& i);
        void str(std::string& strng, int& i);
        static DcmChar *ch(std::string& c, int& i);
        static DcmElem *number(std::string& num, int& i);
        
        // Constructors for the exec parser
        void ex_push(std::string& commands, int& i);
        void ex_pop(std::string& commands, int& i);
        void ex_swap(std::string& commands, int& i);
        void ex_empty(std::string& commands, int& i);
        void ex_peek(std::string& commands, int& i, bool checkScope);
        void ex_attrib(std::string& commands, int& i);
        
    friend class PeekCallback;
    friend class PopCallback;
    friend class PushCallback;
    friend class SwapCallback;
    friend class EmptyCallback;
    friend class AttribCallback;
};

int isEndChar(char c);

int findEnd (std::string& str, int& i);

void skipWhitespace(std::string& str, int& i);
}
#endif
