#ifndef _EXEC_PARSER
#define _EXEC_PARSER
#include "Interpretter.h"

class PushCallback : public Callback {
    private:
        string stkName;
    public:
        PushCallback(string stackName);
        
        Callback *run(Interpretter *interpretter);

        bool mustDestroy();
};


class PopCallback : public Callback {
    private:
        string stkName;
    public:
        PopCallback(string stackName);
        
        Callback *run(Interpretter *interpretter);

        bool mustDestroy();
};

class SwapCallback : public Callback {
    private:
        string stkName;
    public:
        SwapCallback(string stackName);
        
        Callback *run(Interpretter *interpretter);

        bool mustDestroy();
};

class EmptyCallback : public Callback {
    private:
        string stkName;
    public:
        EmptyCallback(string stackName);
        
        Callback *run(Interpretter *interpretter);

        bool mustDestroy();
};

class PeekCallback : public Callback {
    private:
        bool chkScope;
        string stkName;
    public:
        PeekCallback(string stackName, bool checkScope);
        
        Callback *run(Interpretter *interpretter);

        bool mustDestroy();
};

class AttribCallback : public Callback {
    private:
        string stkName;
    public:
        AttribCallback(string stackName);
        
        Callback *run(Interpretter *interpretter);

        bool mustDestroy();
};

#endif
