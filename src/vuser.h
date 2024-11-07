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
        cx AU timeBegin = TimeMain();
        cx AU posDistLinear = Dist(srcPos, dstPos);
        cx AU timeEndLinear = timeBegin + TmMain(posDistLinear / m_cfg.cur.speed);
        cx AU timeDurationLinear = timeEndLinear - timeBegin;
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

        // functions
        cx AU timeCurveFn = [&](F4& tWeighed, F4 t) -> ER {
            switch (m_cfg.cur.timeCurve.type)
            {
            case TimeCurve::Type::LINEAR:
                tWeighed = t;
                break;
            case TimeCurve::Type::ADAM_EASE_1:
                tWeighed = AdamEase1(t, cache.timeCurveRaw);
                break;
            default:
                rete(ErrVal::NO_SUPPORT);
            }
            rets;
        };
        cx AU posCurveFn = [&](Pos2<F4>& nowPos, F4 t) -> ER {
            switch (m_cfg.cur.posCurve.type)
            {
            case PosCurve::Type::LINEAR:
                nowPos = Lerp(srcPos, dstPos, t);
                break;
            case PosCurve::Type::ADAM_HILL_1: {
                cx AU ofsMul = AdamHill1(t, posCurveParam.adamHill1.tMid, posCurveParam.adamHill1.fullness);
                nowPos = Lerp(srcPos, dstPos, t) + cache.posOfsVec * ofsMul;
                break;
            }
            default:
                rete(ErrVal::NO_SUPPORT);
            }
            rets;
        };

        // preprocess pass loop
        struct PathSect
        {
            F4 t;
            F4 dist;

            dfa PathSect(F4 t = F4(0), F4 dist = F4(0)) noex : t(t), dist(dist)
            {
            }
        };
        vector<PathSect> pathSects;
        F4 posDistPath;
        TmMain timeEndPath;
        TmMain timeDurationPath;
        {
            cx AU tTestStepMul = F4(0.9);   // '0.9' allows small curving to fit 'sectDistMax'
            cx AU tRewindNowMul = F4(0.25); // '0.25' rewinds 25% of the step, allows medium curving in 1 retry
            cx AU pathResolution = F4(1.0); // '1.0' means 100% resolution, can be less for performance or more for extra accuracy
            AU posDistSum = F8(0);
            AU tTest = F4(0);
            cx AU stepCntBase = F4(timeDurationLinear / timeWaitBase * pathResolution);
            cx AU sectDistMax = posDistLinear / stepCntBase;
            cx AU tTestStep = F4(1) / stepCntBase * tTestStepMul;
            AU prevPos = srcPos;
            AU tRewind = F4(0);

            pathSects.reserve(SI(F4(1) / tTestStep) + 2);

            do
            {
                AU nowPos = Pos2<F4>(0, 0);
                ife (posCurveFn(nowPos, tTest))
                    retep;

                cx AU sectDist = Dist(prevPos, nowPos);
                if (sectDist > sectDistMax)
                {
                    cx AU tRewindNow = (tTestStep - tRewind) * tRewindNowMul;
                    tTest -= tRewindNow;
                    tRewind += tRewindNow;
                    continue;
                }
                posDistSum += F8(sectDist);

                pathSects.emplace_back(tTest, F4(posDistSum));

                ifu (tTest == F4(1))
                    break;
                tTest += tTestStep;
                ifu (tTest > F4(1))
                    tTest = F4(1);

                tRewind = F4(0);
                prevPos = nowPos;
            } while (YES);

            posDistPath = F4(posDistSum);
            timeEndPath = timeBegin + TmMain(posDistPath / m_cfg.cur.speed);
            timeDurationPath = timeEndPath - timeBegin;

            for (AU& pathSect : pathSects)
                pathSect.dist /= posDistPath;
        }

        // functions
        cx AU timePathFn = [&](F4& tWeighed, F4 t, SI& iCur) -> ER {
            cx AU iEnd = SI(pathSects.size() - 1);
            for (SI i = iCur; i < iEnd; ++i)
            {
                if (IsBetween(t, pathSects[i].dist, pathSects[i + 1].dist))
                {
                    tWeighed = Lerp(pathSects[i].t, pathSects[i + 1].t, (t - pathSects[i].dist) / (pathSects[i + 1].dist - pathSects[i].dist));
                    iCur = i;
                    rets;
                }
            }
            rete(ErrVal::NO_EXIST);
        };

        // transition pass loop
        AU timePathFnICur = SI(0);
        AU timeNow = TmMain(0);
        for (SI i = 0; (timeNow = TimeMain()) < timeEndPath; ++i)
        {
            // get t value
            cx AU tAbs = F4((timeNow - timeBegin) / timeDurationPath);

            // get weighed t value - based on time curve
            AU tWeighed = F4(0);
            ife (timeCurveFn(tWeighed, tAbs))
                retep;

            // get weighed t value - based on path
            ife (timePathFn(tWeighed, tWeighed, timePathFnICur))
                retep;

            // get intermediate position
            AU nowPos = Pos2<F4>(0, 0);
            ife (posCurveFn(nowPos, tWeighed))
                retep;

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
