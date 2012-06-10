#ifndef _EXEC_PARSER
#define _EXEC_PARSER
#include "Interpretter.h"

namespace Dcm {

class PushCallback : public Callback {
    private:
        std::string stkName;
    public:
        PushCallback(std::string stackName);
        
        Callback *run(Interpretter *interpretter);

        bool mustDestroy();
};


class PopCallback : public Callback {
    private:
		std::string stkName;
    public:
        PopCallback(std::string stackName);
        
        Callback *run(Interpretter *interpretter);

        bool mustDestroy();
};

class SwapCallback : public Callback {
    private:
		std::string stkName;
    public:
        SwapCallback(std::string stackName);
        
        Callback *run(Interpretter *interpretter);

        bool mustDestroy();
};

class EmptyCallback : public Callback {
    private:
		std::string stkName;
    public:
        EmptyCallback(std::string stackName);
        
        Callback *run(Interpretter *interpretter);

        bool mustDestroy();
};

class PeekCallback : public Callback {
    private:
        bool chkScope;
        std::string stkName;
    public:
        PeekCallback(std::string stackName, bool checkScope);
        
        Callback *run(Interpretter *interpretter);

        bool mustDestroy();
};

class AttribCallback : public Callback {
    private:
		std::string stkName;
    public:
        AttribCallback(std::string stackName);
        
        Callback *run(Interpretter *interpretter);

        bool mustDestroy();
};
}
#endif
