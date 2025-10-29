#pragma once

#include <vector>
#include <string>
#include "lock.hpp"
#include "vector.hpp"
#include "arg.hpp"

enum class ConCol : U1
{
    BLACK = 0,      // "&0;"
    DARK_BLUE = 1,  // "&1;"
    DARK_GREEN = 2, // "&2;"
    DARK_CYAN = 3,  // "&3;"
    DARK_RED = 4,   // "&4;"
    PURPLE = 5,     // "&5;"
    BROWN = 6,      // "&6;"
    LIGHT_GRAY = 7, // "&7;"
    DARK_GRAY = 8,  // "&8;"
    BLUE = 9,       // "&9;"
    GREEN = 10,     // "&a;"
    CYAN = 11,      // "&b;"
    RED = 12,       // "&c;"
    MAGENTA = 13,   // "&d;"
    YELLOW = 14,    // "&e;"
    WHITE = 15,     // "&f;"
};
enum class ConWaitAnim : U1
{
    STATIC = 0,  // no animation
    SPINNER = 1, // classic spinning line
    DOTS = 2,    // four dots that appear one by one
};

#if defined(PROG_SYS_ESP32)
cxex SI CON_SBO_LENX_MAX = BYTE_IN_KB / 4;
cxex SI CON_HISTORY_CNT_MAX = 16;
#else
cxex SI CON_SBO_LENX_MAX = BYTE_IN_KB;
cxex SI CON_HISTORY_CNT_MAX = 32;
#endif
cxex ConCol CON_COL_DEFA = ConCol::WHITE;

struct ConCtx
{
    ThdLockFast writeLock;
    SI writePosX; // used while (isReadMode == YES)
    BO isReadMode;
    BO isConOwn;
    ConCol colReal;
    ConCol col;
    ConCol colInitSave; // used while (isConOwn == NO)

    dfa ConCtx();
};

extern ConCtx* g_conCtx;

dfa HD _ConOutHdl();
dfa HD _ConInHdl();

dfa ConCol _ConColByCs(CS c);
dfa ER _ConColRealSet(ConCol col);
dfa ER _ConColRealGet(ConCol& col);
dfa ER _ConColUpd();

dfa ER _ConCurPosSet(cx Pos2<SI>& pos);
dfa ER _ConCurPosGet(Pos2<SI>& pos);
dfa ER _ConCurShow(BO isShown);
dfa ER _ConInBufClr();
dfa ER _ConReadModeSet(BO isEnabled);

#ifdef PROG_SYS_WIN

dfa ER _ConWriteRow(cx std::vector<CHAR_INFO>& in, SI y);
dfa ER _ConReadRow(std::vector<CHAR_INFO>& out, SI y);
dfa ER _ConClrRow(SI y);

#endif

dfa ER _ConWriteRaw__(HD hdl, cx CS* buf, SI bufLen);
dfa ER _ConWriteRaw_(cx CS* buf, SI bufLen, BO isInput);
dfa ER _ConWriteRaw(cx CS* buf, SI bufLen, BO isInput);
dfa ER _ConWriteRawAl_(cx CS* form, AL& args);
dfa ER _ConWriteRawAl(cx CS* form, AL& args, cx CS* pre, cx CS* post);

dfa ER _ConReadStr(CS* str, SI strLenxMax, SI& strLen, cx CS* prefix, ThdLockFast* prefixLock, BO isShow);
dfa ER _ConWait(cx CS* prefix, ConWaitAnim anim);

dfa BO _ConIsInit();
dfa ER _ConInit();
dfa ER _ConFree();

dfa BO ConIsExist();
dfa ER ConNew();
dfa ER ConDel();
dfa ER ConReq();

dfa ER ConColSet(ConCol col);
dfa ER ConColGet(ConCol& col);
dfa ER ConTitleSet(cx CH* title);
dfa ER ConTitleGet(std::wstring& title);

dfa ER ConWriteRaw(cx CS* form, ...);
dfa ER ConWrite(cx CS* form, ...);
dfa ER ConWriteDbg(cx CS* form, ...);
dfa ER ConWriteInfo(cx CS* form, ...);
dfa ER ConWriteWarn(cx CS* form, ...);
dfa ER ConWriteErr(cx CS* form, ...);

dfa ER ConReadStr(CS* str, SI strLenxMax, SI& strLen, cx CS* prefix = NUL, ThdLockFast* prefixLock = NUL, BO isShow = YES);
dfa ER ConReadStr(std::string& str, cx CS* prefix = NUL, ThdLockFast* prefixLock = NUL, BO isShow = YES);
dfa ER ConWait(cx CS* prefix, ConWaitAnim anim = ConWaitAnim::STATIC);
dfa ER ConWait();

dfa ER ConBufClr();

#include "console.ipp"
