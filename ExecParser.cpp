/*
    Zachary Weaver
    12/09/2011
    ExecParser.cpp
*/

#include "Interpretter.h"

int isEndChar(char c) {
    switch (c) {
        case ' ':
        case '\t':
        case '.':
        case '[':
        case ']':
        case '#':
            return true;
        default:
            return false;
    }
}

int findEnd (string& str, int& i) {
    int ret;
    while (i < str.size() && !isEndChar(str[i])) i++;
    ret = i - 1;
    if (i >= str.size()) {
        return ret;
    }
    switch (str[i]) {
        // We do this check twice, optimize?
        case '.':
        case '[':
        case ']':
        case '#':
            return ret;
    }
    while (i < str.size() && (str[i] == ' ' || str[i] == '\t')) i++;
    return ret;
}

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
        PeekCallback(string& stackName, bool checkScope) {
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

void Interpretter::exec(string& execStr, int& i) {
    if (strCont != "") {
        strCont += '\n';
        str(commands, i);
        if (i < commands.size()) {
            i++;
            mainStack.push(new DcmString(strCont));
            strCont = "";
        }
    }
    findEnd(commands, i);
    while (i < commands.size()) {
        if (commands[i] <= '9' && commands[i] >= '0') {
            cont.top()->push_back(number(commands, i));
        }
        else switch (commands[i]) {
            case '#':
                return;
            case ']':
                // Close exec
                DcmExec *top;
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
                ex_push(commands, i);
                break;
            case '@':
                ex_pop(commands, i);
                break;
            case '~':
                ex_swap(commands, i);
                break;
            case '?':
                ex_empty(commands, i);
                break;
            case '.':
                ex_attrib(commands, i);
                break;
            case '[':
                cont.push(new DcmExec());
                break;
            case '\"':
                str(commands, ++i);
                if (i < commands.size()) {
                    cont.top()->push_back(new DcmString(strCont));
                    strCont = "";
                    i++;
                }
                break;
            case ',':
                cont.top()->push_back(sym(commands, i));
                break; 
            case '\'':
                cont.top()->push_back(ch(commands, i));
                break;
            case '^':
                ex_peek(commands, ++i, false);
            default:
                ex_peek(commands, i, true);
        }
    }
}

