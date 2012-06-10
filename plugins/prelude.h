#ifndef _PRELUDE_PLUGIN
#define _PRELUDE_PLUGIN

#include "../Interpretter.h"

namespace Dcm {
namespace Prelude {
typedef std::function<double(double, double)> FloatOp;
typedef std::function<int(int, int)> IntOp;
typedef std::function<char(char, char)> CharOp;

Plugin * preludePlugin();

void prelude_addBasic(std::vector<NamedCB>& vec);

void prelude_addControl(std::vector<NamedCB>& vec);

void prelude_addMath(std::vector<NamedCB>& vec);

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

void prelude_addOOP(std::vector<NamedCB>& vec);

void prelude_addStack(std::vector<NamedCB>& vec);

void prelude_addSeq(std::vector<NamedCB>& vec);

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
	std::function<void()> cb;
  public:
    Finally(std::function<void()> initCb);
    ~Finally();
};
}}
#endif
