#ifndef _PRELUDE_PLUGIN
#define _PRELUDE_PLUGIN

#include "../Interpretter.h"

typedef function<double(double, double)> FloatOp;
typedef function<int(int, int)> IntOp;
typedef function<char(char, char)> CharOp;

Plugin * preludePlugin();

void prelude_addBasic(vector<NamedCB>& vec);

void prelude_addControl(vector<NamedCB>& vec);

void prelude_addMath(vector<NamedCB>& vec);

// Some higher level functions from math useful for casting

DcmNum * doIntOp(IntOp intOp,
                 CharOp charOp,
                 DcmType *left,
                 DcmType *right) 
                 throw (DcmTypeError*);

DcmNum * doFloatOp(FloatOp floatOp,
                   IntOp intOp,
                   CharOp charOp,
                   DcmType *left,
                   DcmType *right) 
                   throw (DcmTypeError*);

void prelude_addOOP(vector<NamedCB>& vec);

void prelude_addStack(vector<NamedCB>& vec);

void prelude_addSeq(vector<NamedCB>& vec);

class ExecRunner : public ExecCallback {
  public:
    ExecRunner();
    
    ExecRunner(DcmExec* exec);

    void setExec(DcmExec *exec);
    
    void runExec(Interpretter *interpretter);

    bool mustDestroy();
};

class Finally {
  private:
    function<void()> cb;
  public:
    Finally(function<void()> initCb);
    ~Finally();
};

#endif
