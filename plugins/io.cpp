#include "io.h"
#include <iostream>
#include <string>

using namespace std;

static ostream& dcmout = cout;

static istream& dcmin  = cin;

void cbPrint(DcmStack& stk) {
    DcmType *dcm = safePeekMain(stk);
    stk.pop();
    dcmout << dcm->repr() << endl;
    del(dcm);
}

void cbPStk(DcmStack& stk) {
    stack<DcmType*> items;
    while (!stk.empty()) {
        items.push(stk.top());
        stk.pop();
    }
    while (!items.empty()) {
        dcmout << items.top()->repr() << ' ';
        stk.push(items.top());
        items.pop();
    }
    dcmout << endl;
}

void cbInput(DcmStack& stk) {
    string str;
    getline(dcmin, str);
    stk.push(new DcmString(str));
}

void cbPut(DcmStack& stk) {
    DcmString *dcmStr = static_cast<DcmString*>(
      safePeekMain(stk, {DcmString::typeVal()}));
    stk.pop();
    dcmout << *dcmStr;
    del(dcmStr);
}

void cbPutLine(DcmStack& stk) {
    DcmString *dcmStr = static_cast<DcmString*>(
      safePeekMain(stk, {DcmString::typeVal()}));
    stk.pop();
    dcmout << *dcmStr << endl;
    del(dcmStr);
}

Plugin * ioPlugin() {
    vector<NamedCB> v =
        { NamedCB("print",  new SimpleCallback(cbPrint))
        , NamedCB("p-stk",  new SimpleCallback(cbPStk))
        , NamedCB("input",  new SimpleCallback(cbInput))
        , NamedCB("put",    new SimpleCallback(cbPut))
        , NamedCB("put-ln", new SimpleCallback(cbPutLine))
        };
    return new VectorPlugin(v);
}
