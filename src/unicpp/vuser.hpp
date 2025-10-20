#pragma once

#include "preprocess.hpp"
#ifdef PROG_SYS_WIN
    #include "input.hpp"
    #include "vector.hpp"
#endif

#ifdef PROG_SYS_WIN

class VuserInput
{
  public:
    struct TimeCurve
    {
        enum class Type : U1
        {
            LINEAR = 0,
            ADAM_EASE_1 = 1,
        };

        struct Param
        {
            struct AdamEase1
            {
                F4 curve;
                F4 curveVari;
            };

            union {
                AdamEase1 adamEase1;
            };

            dfa Param();
        };

        Type type;
        Param param;

        dfa TimeCurve();
    };
    struct PosCurve
    {
        enum class Type : U1
        {
            LINEAR = 0,
            ADAM_HILL_1 = 1,
        };

        struct Param
        {
            struct AdamHill1
            {
                F4 tMid;
                F4 fullness;
                F4 h;
                F4 tMidVari;
                F4 fullnessVari;
                F4 hVari;
            };

            union {
                AdamHill1 adamHill1;
            };

            dfa Param();
        };

        Type type;
        Param param;

        dfa PosCurve();
    };

  public:
    struct Cfg
    {
        struct Key
        {
            TmMain downDelay;
            TmMain upDelay;
            TmMain downDelayVari;
            TmMain upDelayVari;

            dfa Key();
        };
        struct Cur
        {
            F4 speed;
            F4 hz;
            TmMain delay;
            TimeCurve timeCurve;
            PosCurve posCurve;
            F4 speedVari;
            F4 hzVari;
            TmMain delayVari;

            dfa Cur();
        };

        Key key;
        Cur cur;

        dfa NT InitAdam();

        dfa Cfg();
    };
    struct State
    {
        struct Key
        {
            dfa Key();
        };
        struct Cur
        {
            Pos2<F4> pos;

            dfa Cur();
        };

        Key key;
        Cur cur;

        dfa State();
    };

  private:
    Cfg m_cfg;
    State m_state;

  public:
    dfa Cfg& CfgGet();

  private:
    dfa NT _CfgKeyVariApply(Cfg::Key& dst, cx Cfg::Key& src);
    dfa NT _CfgCurVariApply(Cfg::Cur& dst, cx Cfg::Cur& src);
    dfa ER _CurPosGet(Pos2<F4>& pos);

  public:
    dfa ER KeyPressDown(InputKey key);
    dfa ER KeyPressUp(InputKey key);
    dfa ER KeyPress(InputKey key);
    dfa ER CurPosSet(cx Pos2<F4>& pos, cx Pos2<F4>& posVari = Pos2<F4>(0, 0));
    dfa ER CurPosMove(cx Pos2<F4>& pos, cx Pos2<F4>& posVari = Pos2<F4>(0, 0));
    dfa ER KeyPressTo(InputKey key, cx Pos2<F4>& pos, cx Pos2<F4>& posVari = Pos2<F4>(0, 0));
    dfa ER KeyDragTo(InputKey key, cx Pos2<F4>& pos, cx Pos2<F4>& posVari = Pos2<F4>(0, 0));
    dfa ER KeyDrag(InputKey key, cx Pos2<F4>& posSrc, cx Pos2<F4>& posDst, cx Pos2<F4>& posSrcVari = Pos2<F4>(0, 0), cx Pos2<F4>& posDstVari = Pos2<F4>(0, 0));

  public:
    dfa VuserInput();
    dfa ~VuserInput();
};

#endif

#include "vuser.ipp"
