#ifndef _PRELUDE_MATH
#define _PRELUDE_MATH

#include "prelude.h"
#include <functional>

void cbAdd(DcmStack& stk);

void cbSub(DcmStack& stk);

void cbMul(DcmStack& stk);

void cbDiv(DcmStack& stk);

void cbMod(DcmStack& stk);

void cbRShift(DcmStack& stk);

void cbLShift(DcmStack& stk);

void cbOr(DcmStack& stk);

void cbAnd(DcmStack& stk);

void cbXor(DcmStack& stk);

void cbNot(DcmStack& stk);

void cbLT(DcmStack& stk);

void cbGT(DcmStack& stk);

void cbLTE(DcmStack& stk);

void cbGTE(DcmStack& stk);

#endif
