#pragma once

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
                F4 tMidVari;
                F4 fullnessVari;
                F4 hVari;
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
        struct Key
        {
            TmMain downDelay;
            TmMain upDelay;
            TmMain downDelayVari;
            TmMain upDelayVari;

            dfa Key()
            {
                downDelay = INPUT_KEY_HOLD_DEFA;
                upDelay = INPUT_KEY_DELAY_DEFA;
                downDelayVari = TmMain(0);
                upDelayVari = TmMain(0);
            }
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

            dfa Cur()
            {
                speed = 1.25f;
                hz = 125.0f;
                delay = INPUT_KEY_DELAY_DEFA;
                speedVari = F4(0);
                hzVari = F4(0);
                delayVari = TmMain(0);
            }
        };

        Key key;
        Cur cur;

        dfa NT InitAdam()
        {
            F4 scnDiagonal;
            ife (ScnDiagonalGet(scnDiagonal))
                scnDiagonal = 2202.907f;
            key.downDelay = 62.7952;
            key.downDelayVari = 13.1827;
            key.upDelay = 92.8447;
            key.upDelayVari = 21.1340;
            cur.speed = 0.00054474f * scnDiagonal;
            cur.speedVari = 0.00011916f * scnDiagonal;
            cur.hz = 250.0f;
            cur.hzVari = F4(0);
            cur.delay = 40.8273;
            cur.delayVari = 4.7361;
            cur.timeCurve.type = VuserInput::TimeCurve::Type::ADAM_EASE_1;
            cur.timeCurve.param.adamEase1.curve = 0.12f;
            cur.timeCurve.param.adamEase1.curveVari = 0.155f;
            cur.posCurve.type = VuserInput::PosCurve::Type::ADAM_HILL_1;
            cur.posCurve.param.adamHill1.tMid = 0.5f;
            cur.posCurve.param.adamHill1.tMidVari = 0.3f;
            cur.posCurve.param.adamHill1.fullness = 0.0f;
            cur.posCurve.param.adamHill1.fullnessVari = 0.75f;
            cur.posCurve.param.adamHill1.h = 0.0f;
            cur.posCurve.param.adamHill1.hVari = 0.25f;
        }

        dfa Cfg()
        {
            tx->InitAdam();
        }
    };
    struct State
    {
        struct Key
        {
            dfa Key()
            {
            }
        };
        struct Cur
        {
            Pos2<F4> pos;

            dfa Cur()
            {
                pos = Pos2<F4>(0.0f, 0.0f);
            }
        };

        Key key;
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
    dfa NT _CfgKeyVariApply(Cfg::Key& dst, cx Cfg::Key& src)
    {
        dst = src;
        if (dst.downDelayVari != TmMain(0))
            dst.downDelay += TmMain(RandF8(-dst.downDelayVari, dst.downDelayVari));
        if (dst.upDelayVari != TmMain(0))
            dst.upDelay += TmMain(RandF8(-dst.upDelayVari, dst.upDelayVari));
    }
    dfa NT _CfgCurVariApply(Cfg::Cur& dst, cx Cfg::Cur& src)
    {
        dst = src;
        if (dst.speedVari != F4(0))
            dst.speed += RandF4(-dst.speedVari, dst.speedVari);
        if (dst.hzVari != F4(0))
            dst.hz += RandF4(-dst.hzVari, dst.hzVari);
        if (dst.delayVari != TmMain(0))
            dst.delay += TmMain(RandF8(-dst.delayVari, dst.delayVari));
        if (dst.timeCurve.type == TimeCurve::Type::ADAM_EASE_1)
        {
            if (dst.timeCurve.param.adamEase1.curveVari != F4(0))
                dst.timeCurve.param.adamEase1.curve += RandF4(-dst.timeCurve.param.adamEase1.curveVari, dst.timeCurve.param.adamEase1.curveVari);
        }
        if (dst.posCurve.type == PosCurve::Type::ADAM_HILL_1)
        {
            if (dst.posCurve.param.adamHill1.tMidVari != F4(0))
                dst.posCurve.param.adamHill1.tMid += RandF4(-dst.posCurve.param.adamHill1.tMidVari, dst.posCurve.param.adamHill1.tMidVari);
            if (dst.posCurve.param.adamHill1.fullnessVari != F4(0))
                dst.posCurve.param.adamHill1.fullness += RandF4(-dst.posCurve.param.adamHill1.fullnessVari, dst.posCurve.param.adamHill1.fullnessVari);
            if (dst.posCurve.param.adamHill1.hVari != F4(0))
                dst.posCurve.param.adamHill1.h += RandF4(-dst.posCurve.param.adamHill1.hVari, dst.posCurve.param.adamHill1.hVari);
        }
    }
    dfa ER _CurPosGet(Pos2<F4>& pos)
    {
        ife (::SysCurPosGet(pos))
            retep;
        rets;
    }

  public:
    dfa ER KeyPressDown(InputKey key)
    {
        Cfg::Key cfgKey;
        tx->_CfgKeyVariApply(cfgKey, m_cfg.key);

        switch (InputKeyTypeGet(key))
        {
        case InputKeyType::KEYB:
            ife (KeybKeyPressDown(key))
                retep;
            break;
        case InputKeyType::MOUS:
            ife (MousKeyPressDown(key))
                retep;
            break;
        default:
            rete(ErrVal::NO_SUPPORT);
        }

        ThdWait(cfgKey.downDelay);

        rets;
    }
    dfa ER KeyPressUp(InputKey key)
    {
        Cfg::Key cfgKey;
        tx->_CfgKeyVariApply(cfgKey, m_cfg.key);

        switch (InputKeyTypeGet(key))
        {
        case InputKeyType::KEYB:
            ife (KeybKeyPressUp(key))
                retep;
            break;
        case InputKeyType::MOUS:
            ife (MousKeyPressUp(key))
                retep;
            break;
        default:
            rete(ErrVal::NO_SUPPORT);
        }

        ThdWait(cfgKey.upDelay);

        rets;
    }
    dfa ER KeyPress(InputKey key)
    {
        ife (tx->KeyPressDown(key))
            retep;
        ife (tx->KeyPressUp(key))
            retep;
        rets;
    }
    dfa ER CurPosSet(cx Pos2<F4>& pos, cx Pos2<F4>& posVari = Pos2<F4>(0, 0))
    {
        // get configuration
        Cfg::Cur cfgCur;
        tx->_CfgCurVariApply(cfgCur, m_cfg.cur);

        // get source position
        Pos2<F4> srcPos;
        ife (tx->_CurPosGet(srcPos))
            retep;

        // get destination position
        cx Pos2<F4> dstPos = pos + ((posVari == Pos2<F4>(0, 0)) ? Pos2<F4>(0, 0) : Pos2<F4>(RandF4(-posVari.x, posVari.x), RandF4(-posVari.y, posVari.y)));

        // calculate constant values #1
        cx AU timeBegin = TimeMain();
        cx AU posDistLinear = Dist(srcPos, dstPos);
        cx AU timeEndLinear = timeBegin + TmMain(posDistLinear / cfgCur.speed);
        cx AU timeDurationLinear = timeEndLinear - timeBegin;
        cx AU timeWaitBase = TmMain(1000) / TmMain(cfgCur.hz);

        // calculate constant values #2
        cx AU& timeCurveParam = cfgCur.timeCurve.param;
        cx AU& posCurveParam = cfgCur.posCurve.param;
        struct Cache
        {
            F4 timeCurveRaw;
            Vec2<F4> posOfsVec;
        };
        Cache cache = {};
        if (cfgCur.timeCurve.type == TimeCurve::Type::ADAM_EASE_1)
            cache.timeCurveRaw = AdamEase1CurveRawGet(timeCurveParam.adamEase1.curve);
        if (cfgCur.posCurve.type == PosCurve::Type::ADAM_HILL_1)
            cache.posOfsVec = Vec2Normalize(Vec2Perp(Vec2Get(srcPos, dstPos))) * (Vec2Len(Vec2Get(srcPos, dstPos)) / 2.0f) * posCurveParam.adamHill1.h;

        // functions
        cx AU timeCurveFn = [&](F4& tWeighed, F4 t) -> ER {
            switch (cfgCur.timeCurve.type)
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
            switch (cfgCur.posCurve.type)
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
        std::vector<PathSect> pathSects;
        F4 posDistPath;
        TmMain timeEndPath;
        TmMain timeDurationPath;
        {
            cx AU tTestStepMul = F4(0.9);   // '0.9' allows small curving to fit 'sectDistMax'
            cx AU tRewindNowMul = F4(0.25); // '0.25' rewinds 25% of the step, allows medium curving in 1 retry
            cx AU pathResolution = F4(1.0); // '1.0' means 100% resolution, can be less for performance or more for extra accuracy
            AU posDistSum = F8(0);
            AU tTest = F4(0);
            cx AU stepCntBase = F4(timeDurationLinear / timeWaitBase * TmMain(pathResolution));
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
            timeEndPath = timeBegin + TmMain(posDistPath / cfgCur.speed);
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
            ife (::SysCurPosSet(nowPos))
                retep;

            // wait for simulated update frequency
            cx AU timeHzTimer = timeBegin + (timeWaitBase * TmMain(i));
            cx AU timeWaitErr = timeWaitBase - (timeNow - timeHzTimer);
            ThdWait(timeWaitErr);
        }

        // set cursor to destination position
        ife (::SysCurPosSet(dstPos))
            retep;

        // delay
        ThdWait(cfgCur.delay);

        // finish
        rets;
    }
    dfa ER CurPosMove(cx Pos2<F4>& pos, cx Pos2<F4>& posVari = Pos2<F4>(0, 0))
    {
        // get source position
        Pos2<F4> srcPos;
        ife (tx->_CurPosGet(srcPos))
            retep;

        // get destination position
        cx Pos2<F4> dstPos = srcPos + pos;

        // set cursor to destination position
        ife (tx->CurPosSet(dstPos, posVari))
            retep;

        // finish
        rets;
    }
    dfa ER KeyPressTo(InputKey key, cx Pos2<F4>& pos, cx Pos2<F4>& posVari = Pos2<F4>(0, 0))
    {
        ife (tx->CurPosSet(pos, posVari))
            retep;
        ife (tx->KeyPress(key))
            retep;
        rets;
    }
    dfa ER KeyDragTo(InputKey key, cx Pos2<F4>& pos, cx Pos2<F4>& posVari = Pos2<F4>(0, 0))
    {
        ife (tx->KeyPressDown(key))
            retep;
        ife (tx->CurPosSet(pos, posVari))
            retep;
        ife (tx->KeyPressUp(key))
            retep;
        rets;
    }
    dfa ER KeyDrag(InputKey key, cx Pos2<F4>& posSrc, cx Pos2<F4>& posDst, cx Pos2<F4>& posSrcVari = Pos2<F4>(0, 0), cx Pos2<F4>& posDstVari = Pos2<F4>(0, 0))
    {
        ife (tx->CurPosSet(posSrc, posSrcVari))
            retep;
        ife (tx->KeyDragTo(key, posDst, posDstVari))
            retep;
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

#endif
