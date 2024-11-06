#pragma once

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
            };

            union {
                AdamEase1 adamEase1;
            };

            dfa Param()
            {
                MemSet(tx, 0, siz(*tx));
            }
        };

        Type type;
        Param param;

        dfa TimeCurve()
        {
            type = Type::LINEAR;
        }
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
            };

            union {
                AdamHill1 adamHill1;
            };

            dfa Param()
            {
                MemSet(tx, 0, siz(*tx));
            }
        };

        Type type;
        Param param;

        dfa PosCurve()
        {
            type = Type::LINEAR;
        }
    };

  public:
    struct Cfg
    {
        struct Cur
        {
            F4 speed;
            F4 hz;
            TimeCurve timeCurve;
            PosCurve posCurve;

            dfa Cur()
            {
                speed = 1.25f;
                hz = 125.0f;
            }
        };

        Cur cur;

        dfa Cfg()
        {
        }
    };
    struct State
    {
        struct Cur
        {
            Pos2<F4> pos;

            dfa Cur()
            {
                pos = Pos2<F4>(0.0f, 0.0f);
            }
        };

        Cur cur;

        dfa State()
        {
        }
    };

  private:
    Cfg m_cfg;
    State m_state;

  public:
    dfa Cfg& CfgGet()
    {
        ret m_cfg;
    }

  private:
    dfa ER _CurPosGet(Pos2<F4>& pos)
    {
        ife (::CurPosGet(pos))
            retep;
        rets;
    }

  public:
    dfa ER CurPosSet(cx Pos2<F4>& pos)
    {
        // get source position
        Pos2<F4> srcPos;
        ife (tx->_CurPosGet(srcPos))
            retep;

        // get destination position
        cx Pos2<F4> dstPos = pos;

        // calculate constant values #1
        cx AU posDist = Dist(srcPos, dstPos);
        cx AU timeBegin = TimeMain();
        cx AU timeEnd = timeBegin + TmMain(posDist / m_cfg.cur.speed);
        cx AU timeDuration = timeEnd - timeBegin;
        cx AU timeWaitBase = TmMain(1000) / TmMain(m_cfg.cur.hz);

        // calculate constant values #2
        cx AU& timeCurveParam = m_cfg.cur.timeCurve.param;
        cx AU& posCurveParam = m_cfg.cur.posCurve.param;
        struct Cache
        {
            F4 timeCurveRaw;
            Vec2<F4> posOfsVec;
        };
        Cache cache = {};

        if (m_cfg.cur.timeCurve.type == TimeCurve::Type::ADAM_EASE_1)
            cache.timeCurveRaw = AdamEase1CurveRawGet(timeCurveParam.adamEase1.curve);
        if (m_cfg.cur.posCurve.type == PosCurve::Type::ADAM_HILL_1)
            cache.posOfsVec = Vec2Normalize(Vec2Perp(Vec2Get(srcPos, dstPos))) * (Vec2Len(Vec2Get(srcPos, dstPos)) / 2.0f) * posCurveParam.adamHill1.h;

        // transition loop
        AU timeNow = TmMain(0);
        for (SI i = 0; (timeNow = TimeMain()) < timeEnd; ++i)
        {
            // get t value
            cx AU tAbs = F4((timeNow - timeBegin) / timeDuration);

            // get weighed t value
            AU tWeighed = F4(0);
            switch (m_cfg.cur.timeCurve.type)
            {
            case TimeCurve::Type::LINEAR:
                tWeighed = tAbs;
                break;
            case TimeCurve::Type::ADAM_EASE_1:
                tWeighed = AdamEase1(tAbs, cache.timeCurveRaw);
                break;
            default:
                rete(ErrVal::NO_SUPPORT);
            }

            // get intermediate position
            AU nowPos = Pos2<F4>(0, 0);
            switch (m_cfg.cur.posCurve.type)
            {
            case PosCurve::Type::LINEAR:
                nowPos = Lerp(srcPos, dstPos, tWeighed);
                break;
            case PosCurve::Type::ADAM_HILL_1: {
                cx AU ofsMul = AdamHill1(tWeighed, posCurveParam.adamHill1.tMid, posCurveParam.adamHill1.fullness);
                nowPos = Lerp(srcPos, dstPos, tWeighed) + cache.posOfsVec * ofsMul;
                break;
            }
            default:
                rete(ErrVal::NO_SUPPORT);
            }

            // set cursor to intermediate position
            ife (::CurPosSet(nowPos))
                retep;

            // wait for simulated update frequency
            cx AU timeHzTimer = timeBegin + (timeWaitBase * TmMain(i));
            cx AU timeWaitErr = timeWaitBase - (timeNow - timeHzTimer);
            ThdWait(timeWaitErr);
        }

        // set cursor to destination position
        ife (::CurPosSet(dstPos))
            retep;

        // finish
        rets;
    }

  public:
    dfa VuserInput()
    {
    }
    dfa ~VuserInput()
    {
    }
};
