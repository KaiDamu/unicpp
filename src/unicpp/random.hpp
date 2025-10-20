#pragma once

#include <vector>
#include "type.hpp"

class RandCtx
{
  public:
    static cxex SI VAL_CNT = 624;

  private:
    SI m_i;
    std::vector<U4> m_val;
    U4 m_seed;
    BO m_isPrep;

  private:
    dfa NT Init(U4 seed);
    dfa NT Prep();
    dfa NT Roll(U4 i11, U4 i12, U4 i21, U4 i22);
    dfa NT Gen();
    dfa U4 Next();

  public:
    dfa F8 RandNormal();
    dfa U4 RandU4();

  private:
    dfa U4 RandU4(U4 min, U4 max);
    dfa S4 RandS4(S4 min, S4 max);
    dfa F4 RandF4(F4 min, F4 max);
    dfa F8 RandF8(F8 min, F8 max);

  public:
    tpl1 dfa T1 RandVal(T1 min, T1 max);

  public:
    dfa RandCtx();
    dfa RandCtx(U4 seed);
};

extern thdlocal RandCtx g_randCtx;

dfa F8 RandNormal();
dfa U4 RandU4();
tpl1 dfa T1 RandVal(T1 min, T1 max);

tpl1 dfa T1 RandMag();

dfa NT RandCrypt(GA buf, SI size);

tpl1 dfa NT MixRand(T1* buf, SI cnt);

#include "random.ipp"
