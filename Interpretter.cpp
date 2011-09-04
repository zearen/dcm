/*
    Zachary Weaver
    03/09/2011
    Interpretter.cpp
*/

// InterpretterError

#include "Interpretter.h"

InterpretterError::InterpretterError(){}

InterpretterError::InterpretterError(string& message) {
    msg = message;
}

InterpretterError::~InterpretterError() throw (){}

const char* InterpretterError::what() {
    if (msg.size() == 0) {
        return "The interpretter encountered an error";
    }
    else {
        return msg.c_str();
    }
}

// Interpretter

Interpretter::Interpretter() {
    scope.push(new Namespace());
}

Interpretter::Interpretter(vector<Plugin> plugins) {
    scope.push(new Namespace());
    for (Plugin p : plugins) {
        addPlugin(p);
    }
}

Interpretter::~Interpretter() {
    Namespace *ns;
    DcmExec *exec;
    while (!scope.empty()) {
        ns = scope.top();
        scope.pop();
        delete ns;
    }
    
    while (!cont.empty()) {
        exec = cont.top();
        scope.pop();
        delete exec;
    }
}

void Interpretter::addPlugin(Plugin& plugin) {
    plugin.link(heaven);
}

// Utility functions for parsing:

bool isEndChar(char c) {
    if (c == ' ' || c == '\t' || c == '.') {
        return true;
    }
    else {
        return false;
    }
}

// returns the end of the token and sets i to the beginning of next token
int findEnd (string& str, int& i) {
    int ret;
    while (i < str.size() && !isEndChar(str[i])) i++;
    ret = i - 1;
    if (i >= str.size()) {
        return ret;
    }
    if (str[i] == '.') {
        return ret;
    }
    while (i < str.size() && (str[i] == ' ' || str[i] == '\t')) i++;
    return ret;
}

void Interpretter::execute(string commands) {
    int i = 0;
    findEnd(commands, i);
    if (!cont.empty()) {
        string lf = "\n";
        cont.top()->append(lf);
        exec(commands, i);
    }
    while (i < commands.size()) {
        if (commands[i] <= '9' && commands[i] >= '0') {
            number(commands, i);
        }
        else switch (commands[i]) {
            case '$':
                push(commands, i);
                break;
            case '@':
                pop(commands, i);
                break;
            case '~':
                swap(commands, i);
                break;
            case '?':
                empty(commands, i);
                break;
            case '.':
                attrib(commands, i);
                break;
            case '[':
                exec(commands, ++i);
                break;
            case '\"':
                str(commands, i);
                break;
            case '\'':
                ch(commands, i);
                break;
            default:
                peek(commands, i);
        }
    }
}

DcmExec *Interpretter::Parse(string& str) {
    
}

// Sub-parsers

void Interpretter::peek(string& stkName, int& i) {
    DcmType *dcm;
    string name;
    int start, end;
    start = ++i;
    end = findEnd(stkName, i);
    name = stkName.substr(start, end);
    if (heaven[name]) {
        heaven[name]->run(this);
    }
    dcm = raw_peek(name, &scope);
    if (dcm) {
        if (*dcm->type() = PRIMFUN) {
            Callback *cb = static_cast<DcmPrimFun*>(dcm)->run(this);
            while (cb = cb->run(this));
        }
        else {
            dcm->addRef();
            mainStack.push(dcm);
        }
    }
}

unsigned char none[] = { 0 };

void Interpretter::pop(string& stkName, int& i) {
    DcmStack *stk;
    DcmType *type;
    string name;
    int start, end;
    start = ++i;
    end = findEnd(stkName, i);
    name = stkName.substr(start, end);

    stk = &(*scope.top())[name];
    if (stk->empty()) {
        throw new DcmStackError(new DcmSymbol(name), none);
    }
    else {
        mainStack.push(stk->top());
        stk->pop();
    }
}

void Interpretter::push(string& stkName, int& i) {
    DcmStack *stk;
    string name;
    int start, end;
    start = ++i;
    
    if (mainStack.empty()) {
        throw new DcmStackError(new DcmSymbol("main stack"), none);
    }
    
    end = findEnd(stkName, i);
    name = stkName.substr(start, end);
    
    stk = &(*scope.top())[name];
    stk->push(mainStack.top());
    mainStack.pop();
}

void Interpretter::swap(string& stkName, int& i) {
    DcmStack *stk;
    string name;
    int start, end;
    start = ++i;
    end = findEnd(stkName, i);
    name = stkName.substr(start, end);
    
    stk = &(*scope.top())[name];
    if (stk->empty()) {
        throw new DcmStackError(new DcmSymbol(name), none);
    }
    else {
        mainStack.swap(*stk);
    }
}

void Interpretter::empty(string& stkName, int& i) {
    string name;
    int start, end;
    start = ++i;
    end = findEnd(stkName, i);
    name = stkName.substr(start, end);
    mainStack.push(new DcmBool((*scope.top())[name].empty()));
}

unsigned char tvNS[] = {NAMESPACE};

void Interpretter::attrib(string& attr, int& i) {         
    DcmType *dcm;
    DcmNamespace *ns;
    
    if (mainStack.empty()) {
        throw new DcmStackError(new DcmSymbol("main stack"), none);
    }
    
    dcm = mainStack.top();
    if (*dcm->type() != NAMESPACE && *dcm->type() != CLASS) {
        throw new DcmTypeError(tvNS, dcm->type());
    }
    mainStack.pop();
    ns = static_cast<DcmNamespace*>(dcm);
    scope.push(ns);
    
    switch (attr[i]) {
        case '@':
            pop(attr, ++i);
            break;
        case '$':
            push(attr, ++i);
            break;
        case '?':
            empty(attr, ++i);
            break;
        case '~':
            swap(attr, ++i);
            break;
        default:
            peek(attr, ++i);
    }
    
    del(ns);
    scope.pop();
}

void Interpretter::exec(string& execStr, int& i) {
    DcmExec *dcmExec;
    string source;
    int start = i;
    if (start >= execStr.size()) {
        cont.push(new DcmExec());
    }
    while (i < execStr.size() && execStr[i] != ']') i++;
    source = execStr.substr(start, i - 1);
    if (i >= execStr.size()) {
        cont.push(Parse(source));
        cont.top()->append(source);
    }
    else {
        if (cont.empty()) {
            dcmExec = Parse(source);
        }
        else {
            dcmExec = cont.top();
            cont.pop();
            for (DcmType *dcm : *Parse(source)) {
                dcmExec->push_back(dcm);
            }
        }
        mainStack.push(dcmExec);
        dcmExec->append(source);
    }
    findEnd(execStr, i);
}

void Interpretter::str(string& strng, int& i) {
}

void Interpretter::ch(string& c, int& i) {
}

void Interpretter::number(string& num, int& i) {
}

