/*
    Zachary Weaver
    03/09/2011
    Interpretter.cpp
*/

// InterpretterError

#include <stdexcept>
#include "Interpretter.h"

InterpretterError::InterpretterError(){}

InterpretterError::InterpretterError(string message) {
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

unsigned char none[] = { 0 };

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

inline void skipWhitespace(string& str, int& i) {
    while (i < str.size() && (str[i] == ' ' || str[i] == '\t')) i++;
}

// returns the length of the token and sets i to the beginning of next token
int findEnd (string& str, int& i) {
    int ret = i;
    while (i < str.size() && !isEndChar(str[i])) i++;
    ret = i - ret;
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
    skipWhitespace(str, i);
    return ret;
}

void Interpretter::execute(string commands) throw (DcmError*){
    int i = 0;
    if (!cont.empty()) {
        static string lf = "\n";
        exec(commands, i);
    }
    if (strCont != "") {
        strCont += '\n';
        str(commands, i);
        if (i < commands.size()) {
            i++;
            mainStack.push(new DcmString(strCont));
            strCont = "";
        }
    }
    skipWhitespace(commands, i);
    //findEnd(commands, i);
    while (i < commands.size()) {
        if (commands[i] <= '9' && commands[i] >= '0') {
            mainStack.push(number(commands, i));
        }
        else switch (commands[i]) {
            case '#':
                return;
            case ']':
                throw InterpretterError("Unexpected ]");
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
                cont.push(new DcmExec());
                exec(commands, ++i);
                skipWhitespace(commands, i);
                break;
            case '\"':
                str(commands, ++i);
                if (i < commands.size()) {
                    mainStack.push(new DcmString(strCont));
                    strCont = "";
                    i++;
                    skipWhitespace(commands, i);
                }
                break;
            case ',':
                mainStack.push(sym(commands, i));
                break;
            case '\'':
                mainStack.push(ch(commands, i));
                break;
            case '^':
                peek(commands, ++i, false);
                break;
            default:
                peek(commands, i, true);
        }
    }
}

// Sub-parsers

void Interpretter::peek(string& stkName, int& i, bool checkHeaven) {
    DcmType *dcm = NULL;
    string name;
    int start, end;
    start = i;
    end = findEnd(stkName, i);
    name = stkName.substr(start, end);
    
    if (checkHeaven) {
        DcmStack stk;
        try {
            stk = heaven.at(name);
            if (stk.empty()) {
                dcm = raw_peek(name, &scope);
            }
            else {
                dcm = stk.top();
            }
        }
        catch (out_of_range e) {
            dcm = raw_peek(name, &scope);
        }
    }
    else {
        dcm = raw_peek(name, &scope);
    }
    if (dcm) {
        if (checkHeaven && *dcm->type() == PRIMFUN) {
            Callback *cb = static_cast<DcmPrimFun*>(dcm)->run(this);
            callbackLoop(cb, this);
        }
        else {
            dcm->addRef();
            mainStack.push(dcm);
        }
    }
    else {
        throw new DcmStackError(new DcmSymbol(name), none);
    }
}

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
    mainStack.swap(*stk);
}

void Interpretter::empty(string& stkName, int& i) {
    string name;
    int start, end;
    start = ++i;
    end = findEnd(stkName, i);
    name = stkName.substr(start, end);
    mainStack.push(new DcmBool(!(*scope.top())[name].empty()));
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
            peek(attr, ++i, false);
    }
    
    del(ns);
    scope.pop();
}

int parseNum(string& num, int& i) {
    int ret = 0;
    while (i < num.size() && '9' >= num[i] && '0' <= num[i]) {
        ret = ret * 10 + (num[i] - '0');
        i++;
    }
    findEnd(num, i);
    return ret;
}

char hex2val(char ch) {
    if ('9' >= ch && '1' <= ch) {
        return ch - '0';
    }
    else if ('a' <= ch && ch <= 'f') {
        return ch - 87; // -87 = -'a' + 10
    }
    else if ('A' <= ch && ch <= 'F') {
        return ch - 55; // -55 = - 'A'
    }
    else {
        return 0;
    }
}

int parseHex(string& hex, int& i) {
    int ret = 0;
    char val;
    for(;i < hex.size(); i++) {
        val = hex2val(hex[i]);
        if (val) {
            ret = (ret << 4) + val;
        }
        else if (hex[i] == '0') {
                ret <<= 4;
        }
        else {
            return ret;
        }
    }
    findEnd(hex, i);
    return ret;
}

char parseSpecialChar(string& chstr, int& i) {
    if (i >= chstr.size()) {
        return '\n';
    }
    if ('9' >= chstr[i] && chstr[i] <= '0') {
        return (char) parseNum(chstr, i);
    }
    i++;
    switch (chstr[i-1]) {
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'x':
            return (char) parseHex(chstr, ++i);
        case 'a':
            return '\a';
        defualt:
            return chstr[i-1];
    }
}

DcmSymbol *Interpretter::sym(string& symName, int& i) {
    string name;
    int start, end;
    start = ++i;
    end = findEnd(symName, i);
    name = symName.substr(start, end);
    return new DcmSymbol(name);
}

void Interpretter::str(string& strng, int& i) {
    for (;i < strng.size() && strng[i] != '\"'; i++) {
        if (strng[i] == '\\') {
            strCont += parseSpecialChar(strng, ++i);
        }
        else {
            strCont += strng[i];
        }
    }
}

DcmChar *Interpretter::ch(string& c, int& i) {
    DcmChar *dcmch;
    i++;
    if (i >= c.size()) {
        return new DcmChar('\n');
    }
    else if (c[i] == '\\') {
        i++;
        dcmch = new DcmChar(parseSpecialChar(c, i));
    }
    else {
        i++;
        dcmch = new DcmChar(c[i-1]);
    }
    skipWhitespace(c, i);
    return dcmch;
}

DcmElem *Interpretter::number(string& num, int& i) {
    int ret=0;
    if (num[i] == '0') {
        i++;
        if (num[i] == 'x') {
            ret = parseHex(num, ++i);
        }
        else {
            ret = parseNum(num, i);
        }
    }
    else {
        int a;
        a = 1;
        ret = parseNum(num, i);
        a += 3;
    }
    if (i >= num.size()) {
        return new DcmInt(ret);
    }
    if (num[i] == '.') {
        // Parse as float
        double fret = ret;
        double multiplier = 1;
        i++;
        for(; i < num.size() && '9' >= num[i] && '0' <= num[i]; i++) {
            multiplier /= 10;
            fret += (num[i] - '0') * multiplier;
        }
        skipWhitespace(num, i);
        return new DcmFloat(fret);
    }
    else {
        return new DcmInt(ret);
    }
}

