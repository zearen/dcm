/*
    Zachary Weaver
    12/09/2011
    ExecParser.cpp
*/

#include "Interpretter.h"
#include "ExecParser.h"

PushCallback::PushCallback(string stackName) {
    stkName = stackName;
}

Callback *PushCallback::run(Interpretter *interpretter) {
    // The parser will increment this so it begins at the
    //+ start of the string (as opposed to one after)
    int i = -1;
    interpretter->push(stkName, i);
    return NULL;
}

bool PushCallback::mustDestroy() {
    return true;
}

void Interpretter::ex_push(string& commands, int& i) {
    int start , end;
    start = i;
    end = findEnd(commands, i);
    cont.top()->push_back(new DcmPrimFun(new PushCallback(
        commands.substr(start, end))));
}

PopCallback::PopCallback(string stackName) {
    stkName = stackName;
}

Callback *PopCallback::run(Interpretter *interpretter) {
    // The parser will increment this so it begins at the
    //+ start of the string (as opposed to one after)
    int i = -1;
    interpretter->pop(stkName, i);
    return NULL;
}
bool PopCallback::mustDestroy() {
    return true;
}

void Interpretter::ex_pop(string& commands, int& i) {
    int start, end;
    start = i;
    end = findEnd(commands, i);
    cont.top()->push_back(new DcmPrimFun(new PopCallback(
        commands.substr(start, end))));
}

SwapCallback::SwapCallback(string stackName) {
    stkName = stackName;
}

Callback *SwapCallback::run(Interpretter *interpretter) {
    // The parser will increment this so it begins at the
    //+ start of the string (as opposed to one after)
    int i = -1;
    interpretter->swap(stkName, i);
    return NULL;
}
bool SwapCallback::mustDestroy() {
    return true;
}

void Interpretter::ex_swap(string& commands, int& i) {
    int start, end;
    start = i;
    end = findEnd(commands, i);
    cont.top()->push_back(new DcmPrimFun(new SwapCallback(
        commands.substr(start, end))));
}

EmptyCallback::EmptyCallback(string stackName) {
    stkName = stackName;
}

Callback *EmptyCallback::run(Interpretter *interpretter) {
    int i = 0;
    interpretter->empty(stkName, i);
    return NULL;
}
bool EmptyCallback::mustDestroy() {
    return true;
}

void Interpretter::ex_empty(string& commands, int& i) {
    int start, end;
    start = i;
    end = findEnd(commands, i);
    cont.top()->push_back(new DcmPrimFun(new EmptyCallback(
        commands.substr(start, end))));
}

PeekCallback::PeekCallback(string stackName, bool checkScope) {
    stkName = stackName;
    chkScope = checkScope;
}

Callback *PeekCallback::run(Interpretter *interpretter) {
    int i = 0;
    interpretter->peek(stkName, i, chkScope);
    return NULL;
}
bool PeekCallback::mustDestroy() {
    return true;
}

void Interpretter::ex_peek(string& commands, int& i, bool checkScope) {
    int start, end;
    start = i;
    end = findEnd(commands, i);
    cont.top()->push_back(new DcmPrimFun(new PeekCallback(
        commands.substr(start, end), checkScope)));
}

AttribCallback::AttribCallback(string stackName) {
    stkName = stackName;
}

Callback *AttribCallback::run(Interpretter *interpretter) {
    int i = 0;
    interpretter->attrib(stkName, i);
    return NULL;
}
bool AttribCallback::mustDestroy() {
    return true;
}

void Interpretter::ex_attrib(string& commands, int& i) {
    int start, end;
    start = i;
    end = findEnd(commands, i);
    cont.top()->push_back(new DcmPrimFun(new AttribCallback(
        commands.substr(start, end))));
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
    skipWhitespace(execStr, i);
    while (i < execStr.size()) {
        if (execStr[i] <= '9' && execStr[i] >= '0'
            || execStr[i] == '-') {
            DcmElem* num = number(execStr, i);
            if (num) {
                cont.top()->push_back(num);
                continue;
            }
        }
        switch (execStr[i]) {
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
                    skipWhitespace(execStr, i);
                    cont.top()->push_back(top);
                }
                break;
            case '$':
                ex_push(execStr, ++i);
                break;
            case '@':
                ex_pop(execStr, ++i);
                break;
            case '~':
                ex_swap(execStr, ++i);
                break;
            case '?':
                ex_empty(execStr, ++i);
                break;
            case '.':
                ex_attrib(execStr, ++i);
                break;
            case '[':
                cont.push(new DcmExec());
                skipWhitespace(execStr, ++i);
                break;
            case '\"':
                str(execStr, ++i);
                if (i < execStr.size()) {
                    cont.top()->push_back(new DcmString(strCont));
                    strCont = "";
                    i++;
                }
                skipWhitespace(execStr, i);
                break;
            case ',':
                cont.top()->push_back(sym(execStr, i));
                break; 
            case '\'':
                cont.top()->push_back(ch(execStr, i));
                break;
            case '^':
                ex_peek(execStr, ++i, false);
                break;
            default:
                ex_peek(execStr, i, true);
        }
    }
}

