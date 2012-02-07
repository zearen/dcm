/*
    Zachary Weaver
    12/09/2011
    ExecParser.cpp
*/

#include "Interpretter.h"
class PushCallback : public Callback {
    private:
        string stkName;
    public:
        PushCallback(string stackName) {
            stkName = stackName;
        }
        
        Callback *run(Interpretter *interpretter) {
            // The parser will increment this so it begins at the
            //+ start of the string (as opposed to one after)
            int i = -1;
            interpretter->push(stkName, i);
        }
};

void Interpretter::ex_push(string& commands, int& i) {
    int start , end;
    start = i;
    end = findEnd(commands, i);
    cont.top()->push_back(new DcmPrimFun(new PushCallback(
        commands.substr(start, end)), true));
}

class PopCallback : public Callback {
    private:
        string stkName;
    public:
        PopCallback(string stackName) {
            stkName = stackName;
        }
        
        Callback *run(Interpretter *interpretter) {
            // The parser will increment this so it begins at the
            //+ start of the string (as opposed to one after)
            int i = -1;
            interpretter->push(stkName, i);
        }
};

void Interpretter::ex_pop(string& commands, int& i) {
    int start, end;
    start = i;
    end = findEnd(commands, i);
    cont.top()->push_back(new DcmPrimFun(new PopCallback(
        commands.substr(start, end)), true));
}

class SwapCallback : public Callback {
    private:
        string stkName;
    public:
        SwapCallback(string stackName) {
            stkName = stackName;
        }
        
        Callback *run(Interpretter *interpretter) {
            // The parser will increment this so it begins at the
            //+ start of the string (as opposed to one after)
            int i = -1;
            interpretter->swap(stkName, i);
        }
};

void Interpretter::ex_swap(string& commands, int& i) {
    int start, end;
    start = i;
    end = findEnd(commands, i);
    cont.top()->push_back(new DcmPrimFun(new SwapCallback(
        commands.substr(start, end)), true));
}

class EmptyCallback : public Callback {
    private:
        string stkName;
    public:
        EmptyCallback(string stackName) {
            stkName = stackName;
        }
        
        Callback *run(Interpretter *interpretter) {
            int i = 0;
            interpretter->empty(stkName, i);
        }
};

void Interpretter::ex_empty(string& commands, int& i) {
    int start, end;
    start = i;
    end = findEnd(commands, i);
    cont.top()->push_back(new DcmPrimFun(new EmptyCallback(
        commands.substr(start, end)), true));
}

class PeekCallback : public Callback {
    private:
        bool chkScope;
        string stkName;
    public:
        PeekCallback(string stackName, bool checkScope) {
            stkName = stackName;
            chkScope = checkScope;
        }
        
        Callback *run(Interpretter *interpretter) {
            int i = 0;
            interpretter->peek(stkName, i, chkScope);
        }
};

void Interpretter::ex_peek(string& commands, int& i, bool checkScope) {
    int start, end;
    start = i;
    end = findEnd(commands, i);
    cont.top()->push_back(new DcmPrimFun(new PeekCallback(
        commands.substr(start, end), checkScope), true));
}

class AttribCallback : public Callback {
    private:
        string stkName;
    public:
        AttribCallback(string stackName) {
            stkName = stackName;
        }
        
        Callback *run(Interpretter *interpretter) {
            int i = 0;
            interpretter->attrib(stkName, i);
        }
};

void Interpretter::ex_attrib(string& commands, int& i) {
    int start, end;
    start = i;
    end = findEnd(commands, i);
    cont.top()->push_back(new DcmPrimFun(new AttribCallback(
        commands.substr(start, end)), true));
}

void Interpretter::exec(string& execStr, int& i) {
    if (strCont != "") {
        strCont += '\n';
        str(execStr, i);
        if (i < execStr.size()) {
            i++;
            mainStack.push(new DcmString(strCont));
            strCont = "";
        }
    }
    findEnd(execStr, i);
    while (i < execStr.size()) {
        if (execStr[i] <= '9' && execStr[i] >= '0') {
            cont.top()->push_back(number(execStr, i));
        }
        else switch (execStr[i]) {
            case '#':
                return;
            case ']':
                // Close exec
                DcmExec *top;
                i++;
                top = cont.top();
                cont.pop();
                if (cont.empty()) {
                    mainStack.push(top);
                    return;
                }
                else {
                    cont.top()->push_back(top);
                }
                break;
            case '$':
                ex_push(execStr, i);
                break;
            case '@':
                ex_pop(execStr, i);
                break;
            case '~':
                ex_swap(execStr, i);
                break;
            case '?':
                ex_empty(execStr, i);
                break;
            case '.':
                ex_attrib(execStr, i);
                break;
            case '[':
                cont.push(new DcmExec());
                break;
            case '\"':
                str(execStr, ++i);
                if (i < execStr.size()) {
                    cont.top()->push_back(new DcmString(strCont));
                    strCont = "";
                    i++;
                }
                break;
            case ',':
                cont.top()->push_back(sym(execStr, i));
                break; 
            case '\'':
                cont.top()->push_back(ch(execStr, i));
                break;
            case '^':
                ex_peek(execStr, ++i, false);
            default:
                ex_peek(execStr, i, true);
        }
    }
}

