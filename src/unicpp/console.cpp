#include "console.hpp"

#include "string.hpp"
#include "array.hpp"
#include "window.hpp"
#include "thread.hpp"
#include "memory.hpp"
#ifdef PROG_SYS_LINUX
    #include <unistd.h>
#endif

dfa ConCtx::ConCtx() : writePosX(0), isReadMode(NO), isConOwn(NO), colReal(CON_COL_DEFA), col(CON_COL_DEFA), colInitSave(CON_COL_DEFA)
{
}

ConCtx* g_conCtx = NUL;

dfa HD _ConOutHdl()
{
#ifdef PROG_SYS_WIN
    ret GetStdHandle(STD_OUTPUT_HANDLE);
#else
    ret NUL;
#endif
}
dfa HD _ConInHdl()
{
#ifdef PROG_SYS_WIN
    ret GetStdHandle(STD_INPUT_HANDLE);
#else
    ret NUL;
#endif
}

dfa ConCol _ConColByCs(CS c)
{
    switch (c)
    {
    case '0':
        ret ConCol::BLACK;
    case '1':
        ret ConCol::DARK_BLUE;
    case '2':
        ret ConCol::DARK_GREEN;
    case '3':
        ret ConCol::DARK_CYAN;
    case '4':
        ret ConCol::DARK_RED;
    case '5':
        ret ConCol::PURPLE;
    case '6':
        ret ConCol::BROWN;
    case '7':
        ret ConCol::LIGHT_GRAY;
    case '8':
        ret ConCol::DARK_GRAY;
    case '9':
        ret ConCol::BLUE;
    case 'a':
        ret ConCol::GREEN;
    case 'b':
        ret ConCol::CYAN;
    case 'c':
        ret ConCol::RED;
    case 'd':
        ret ConCol::MAGENTA;
    case 'e':
        ret ConCol::YELLOW;
    case 'f':
        ret ConCol::WHITE;
    default:
        ret CON_COL_DEFA;
    }
}
dfa ER _ConColRealSet(ConCol col)
{
#if defined(PROG_SYS_WIN)
    cx AU hdl = _ConOutHdl();
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::NO_EXIST);
    ifu (SetConsoleTextAttribute(hdl, WORD(col)) == 0)
        rete(ErrVal::CON);
    g_conCtx->colReal = col;
    rets;
#elif defined(PROG_SYS_LINUX)
    rets; // TODO: implement color setting for Linux console
#elif defined(PROG_SYS_ESP32)
    rets; // assume no color support & ignore the request
#else
    rete(ErrVal::NO_SUPPORT);
#endif
}
dfa ER _ConColRealGet(ConCol& col)
{
#if defined(PROG_SYS_WIN)
    cx AU hdl = _ConOutHdl();
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::NO_EXIST);
    CONSOLE_SCREEN_BUFFER_INFO info;
    ifu (GetConsoleScreenBufferInfo(hdl, &info) == 0)
        rete(ErrVal::CON);
    col = ConCol(info.wAttributes & 0x000F);
    rets;
#elif defined(PROG_SYS_LINUX)
    col = CON_COL_DEFA; // TODO: implement color getting for Linux console
    rets;
#elif defined(PROG_SYS_ESP32)
    col = CON_COL_DEFA; // assume no color support & return default color
    rets;
#else
    rete(ErrVal::NO_SUPPORT);
#endif
}
dfa ER _ConColUpd()
{
    if (g_conCtx->colReal != g_conCtx->col)
        ife (_ConColRealSet(g_conCtx->col))
            retep;
    rets;
}

dfa ER _ConCurPosSet(cx Pos2<SI>& pos)
{
#ifdef PROG_SYS_WIN
    cx AU hdl = _ConOutHdl();
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::NO_EXIST);
    cx COORD tmp = {SHORT(pos.x), SHORT(pos.y)};
    ifu (SetConsoleCursorPosition(hdl, tmp) == 0)
        rete(ErrVal::CON);
    rets;
#else
    rete(ErrVal::NO_SUPPORT);
#endif
}
dfa ER _ConCurPosGet(Pos2<SI>& pos)
{
#ifdef PROG_SYS_WIN
    cx AU hdl = _ConOutHdl();
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::NO_EXIST);
    CONSOLE_SCREEN_BUFFER_INFO info;
    ifu (GetConsoleScreenBufferInfo(hdl, &info) == 0)
        rete(ErrVal::CON);
    pos.x = SI(info.dwCursorPosition.X);
    pos.y = SI(info.dwCursorPosition.Y);
    rets;
#else
    rete(ErrVal::NO_SUPPORT);
#endif
}
dfa ER _ConCurShow(BO isShown)
{
#if defined(PROG_SYS_WIN)
    cx AU hdl = _ConOutHdl();
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::NO_EXIST);
    CONSOLE_CURSOR_INFO info;
    ifu (GetConsoleCursorInfo(hdl, &info) == 0)
        rete(ErrVal::CON);
    info.bVisible = BOOL(isShown);
    ifu (SetConsoleCursorInfo(hdl, &info) == 0)
        rete(ErrVal::CON);
    rets;
#elif defined(PROG_SYS_LINUX)
    rets; // TODO: implement cursor visibility control for Linux console
#elif defined(PROG_SYS_ESP32)
    rets; // assume no cursor visibility control & ignore the request
#else
    rete(ErrVal::NO_SUPPORT);
#endif
}
dfa ER _ConInBufClr()
{
#if defined(PROG_SYS_WIN)
    cx AU hdl = _ConInHdl();
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::NO_EXIST);
    INPUT_RECORD ir;
    DWORD irRead;
    while (YES)
    {
        ifu (PeekConsoleInputW(hdl, &ir, 1, &irRead) == 0)
            rete(ErrVal::CON);
        if (irRead == 0)
            break;
        ifu (ReadConsoleInputW(hdl, &ir, 1, &irRead) == 0)
            rete(ErrVal::CON);
    }
    rets;
#elif defined(PROG_SYS_LINUX)
    rets; // TODO: implement input buffer clearing for Linux console
#elif defined(PROG_SYS_ESP32)
    rets; // assume this is not possible & ignore the request
#else
    rete(ErrVal::NO_SUPPORT);
#endif
}
dfa ER _ConReadModeSet(BO isEnabled)
{
    ife (ConReq())
        retep;
    if (isEnabled)
    {
        Pos2<SI> pos;
        ife (_ConCurPosGet(pos))
            retep;
        g_conCtx->writePosX = pos.x;
        if (g_conCtx->writePosX != 0)
            ife (_ConWriteRaw("\n", 1, NO))
                retep;
    }
    g_conCtx->isReadMode = isEnabled;
    ife (_ConCurShow(isEnabled))
        retep;
    rets;
}

#ifdef PROG_SYS_WIN

dfa ER _ConWriteRow(cx std::vector<CHAR_INFO>& in, SI y)
{
    #ifdef PROG_SYS_WIN
    cx AU hdl = _ConOutHdl();
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::NO_EXIST);
    cx COORD bufSize = {SHORT(in.size()), 1};
    cx COORD pos = {0, 0};
    SMALL_RECT area = {0, SHORT(y), SHORT(in.size() - 1), SHORT(y)};
    ifu (WriteConsoleOutputA(hdl, in.data(), bufSize, pos, &area) == 0)
        rete(ErrVal::CON);
    rets;
    #else
    rete(ErrVal::NO_SUPPORT);
    #endif
}
dfa ER _ConReadRow(std::vector<CHAR_INFO>& out, SI y)
{
    #ifdef PROG_SYS_WIN
    out.clear();
    cx AU hdl = _ConOutHdl();
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::NO_EXIST);
    CONSOLE_SCREEN_BUFFER_INFO info;
    ifu (GetConsoleScreenBufferInfo(hdl, &info) == 0)
        rete(ErrVal::CON);
    out.resize(info.dwSize.X);
    cx COORD bufSize = {SHORT(out.size()), 1};
    cx COORD pos = {0, 0};
    SMALL_RECT area = {0, SHORT(y), SHORT(out.size() - 1), SHORT(y)};
    ifu (ReadConsoleOutputA(hdl, out.data(), bufSize, pos, &area) == 0)
    {
        out.clear();
        rete(ErrVal::CON);
    }
    rets;
    #else
    rete(ErrVal::NO_SUPPORT);
    #endif
}
dfa ER _ConClrRow(SI y)
{
    std::vector<CHAR_INFO> buf;
    ife (_ConReadRow(buf, y))
        retep;
    for (AU& elem : buf)
        elem.Char.AsciiChar = ' ';
    ife (_ConWriteRow(buf, y))
        retep;
    rets;
}

#endif

dfa ER _ConWriteRaw__(HD hdl, cx CS* buf, SI bufLen)
{
    if (bufLen < 1)
        rets;
#ifdef PROG_SYS_WIN
    DWORD result;
    ifu ((WriteConsoleA(hdl, buf, DWORD(bufLen), &result, NUL) == 0) || (SI(result) != bufLen))
        rete(ErrVal::CON);
#else
    cx AU result = fwrite(buf, 1, bufLen, stdout);
    ifu (SI(result) != bufLen)
        rete(ErrVal::CON);
    unused(hdl);
#endif
    rets;
}
dfa ER _ConWriteRaw_(cx CS* buf, SI bufLen, BO isInput)
{
    cx AU hdl = _ConOutHdl();
#if defined(PROG_SYS_WIN)
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::NO_EXIST);
#endif
    ife (_ConColUpd())
        retep;
    cx AU bufEnd = buf + bufLen;
    AU bufCur = buf;
    ConCol colOld;
    ife (ConColGet(colOld))
        retep;

#if defined(PROG_SYS_WIN)
    Pos2<SI> posWritePre;
    Pos2<SI> posInput;
    std::vector<CHAR_INFO> rowInputSave;
    cx AU doWriteRedir = BO(g_conCtx->isReadMode && !isInput);
    if (doWriteRedir)
    {
        ife (_ConCurPosGet(posInput))
            retep;
        ife (_ConReadRow(rowInputSave, posInput.y))
            retep;
        std::vector<CHAR_INFO> rowInputClr = rowInputSave;
        for (AU& elem : rowInputClr)
            elem.Char.AsciiChar = ' ';
        ife (_ConWriteRow(rowInputClr, posInput.y))
            retep;
        posWritePre = Pos2<SI>(g_conCtx->writePosX, posInput.y - (g_conCtx->writePosX != 0));
        ife (_ConCurPosSet(posWritePre))
            retep;
    }
#endif

    while (bufCur != bufEnd)
    {
        AU first = bufCur;
        AU last = TO(first)(NUL);
        while ((bufCur + 2 < bufEnd) && !((*bufCur == '&') && (*(bufCur + 2) == ';')))
            ++bufCur;
        ConCol colNew;
        BO isColNew = NO;
        if (bufCur + 2 < bufEnd)
        {
            isColNew = YES;
            cx AU code = *(bufCur + 1);
            colNew = _ConColByCs(code);
            last = bufCur;
            bufCur += 3;
        }
        else
        {
            last = bufEnd;
            bufCur = bufEnd;
        }
        ife (_ConWriteRaw__(hdl, first, SI(last - first)))
            retep;
        if (isColNew)
        {
            ife (ConColSet(colNew))
                retep;
            ife (_ConColUpd())
                retep;
        }
    }

#if defined(PROG_SYS_WIN)
    if (doWriteRedir)
    {
        Pos2<SI> posWritePost;
        ife (_ConCurPosGet(posWritePost))
            retep;
        g_conCtx->writePosX = posWritePost.x;
        if (g_conCtx->writePosX != 0)
        {
            ife (_ConWriteRaw__(hdl, "\n", 1))
                retep;
            ife (_ConCurPosGet(posWritePost))
                retep;
        }
        ife (_ConWriteRow(rowInputSave, posWritePost.y))
            retep;
        posInput.y = posWritePost.y;
        ife (_ConCurPosSet(posInput))
            retep;
    }
#endif

    ife (ConColSet(colOld))
        retep;
    rets;
}
dfa ER _ConWriteRaw(cx CS* buf, SI bufLen, BO isInput)
{
    ifu (bufLen <= 0)
        rets;
    g_conCtx->writeLock.Lock();
    cx AU err = _ConWriteRaw_(buf, bufLen, isInput);
    g_conCtx->writeLock.Unlock();
    ret err;
}
dfa ER _ConWriteRawAl_(cx CS* form, AL& args)
{
    ArrSbo<CS, CON_SBO_LENX_MAX> buf;
    jdst(retry);
    cx AU bufLenResult = SI(vsnprintf(buf.Dat(), buf.Cnt() - STR_EX_LEN, form, args));
    ifu (bufLenResult < 0)
        rete(ErrVal::CON);
    ifu (bufLenResult >= buf.Cnt() - STR_EX_LEN)
    {
        buf.Resize(buf.Cnt() * 4, 0);
        jsrc(retry);
    }
    ife (_ConWriteRaw(buf.Dat(), bufLenResult, NO))
        retep;
    rets;
}
dfa ER _ConWriteRawAl(cx CS* form, AL& args, cx CS* pre, cx CS* post)
{
    ife (ConReq())
        retep;
    std::string form_;
    if (pre != NUL)
        form_ += pre;
    form_ += form;
    if (post != NUL)
        form_ += post;
    ret _ConWriteRawAl_(form_.c_str(), args);
}

dfa ER _ConReadStr(CS* str, SI strLenxMax, SI& strLen, cx CS* prefix, ThdLockFast* prefixLock, BO isShow)
{
#ifdef PROG_SYS_WIN
    strLen = -1;
    ifu (strLenxMax < STR_EX_LEN)
        rete(ErrVal::LOW_SIZE);
    strLen = 0;
    str[strLen] = '\0';
    ife (_ConInBufClr())
        retep;
    cx AU hdl = _ConInHdl();
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::CON);
    SI strPos = 0;
    static std::vector<std::string>* s_history = NUL;
    if (s_history == NUL)
        s_history = new std::vector<std::string>;
    SI historyPos = -1;
    std::string prefixLast;
    SI prefixLastLenShow;
    if (prefix != NUL)
    {
        if (prefixLock != NUL)
            prefixLock->Lock();
        prefixLast = prefix;
        if (prefixLock != NUL)
            prefixLock->Unlock();
        ife (_ConWriteRaw(prefixLast.c_str(), prefixLast.size(), YES))
            retep;
        if (prefixLock != NUL)
        {
            Pos2<SI> curPos;
            ife (_ConCurPosGet(curPos))
                retep;
            prefixLastLenShow = curPos.x;
        }
    }
    while (YES)
    {
        if (prefixLock != NUL && prefix != NUL)
        {
            prefixLock->Lock();
            ifu (StrCmp(prefixLast.c_str(), prefix) != 0)
            {
                Pos2<SI> curPos;
                ife (_ConCurPosGet(curPos))
                    retep;
                std::vector<CHAR_INFO> rowDatOld;
                ife (_ConReadRow(rowDatOld, curPos.y))
                    retep;
                cx Pos2<SI> curPosHome(0, curPos.y);
                ife (_ConCurPosSet(curPosHome))
                    retep;
                ife (_ConWriteRaw(prefix, StrLen(prefix), YES))
                    retep;
                Pos2<SI> curPosAfterWrite;
                ife (_ConCurPosGet(curPosAfterWrite))
                    retep;
                cx AU prefixLenShow = curPosAfterWrite.x;
                std::vector<CHAR_INFO> rowDatNew;
                ife (_ConReadRow(rowDatNew, curPos.y))
                    retep;
                rowDatNew.resize(prefixLenShow);
                cx AU cpyCnt = SI(rowDatOld.size()) - Max(prefixLenShow, prefixLastLenShow);
                ite (i, i < cpyCnt)
                    rowDatNew.emplace_back(rowDatOld[i + prefixLastLenShow]);
                ife (_ConWriteRow(rowDatNew, curPos.y))
                    retep;
                cx Pos2<SI> curPosNew(curPos.x + (prefixLenShow - prefixLastLenShow), curPos.y);
                ife (_ConCurPosSet(curPosNew))
                    retep;
                prefixLast = prefix;
                prefixLastLenShow = prefixLenShow;
            }
            prefixLock->Unlock();
        }
        INPUT_RECORD ir;
        DWORD irRead;
        ifu (PeekConsoleInputW(hdl, &ir, 1, &irRead) == 0)
            rete(ErrVal::CON);
        if (irRead == 0)
        {
            ThdWait(10);
            continue;
        }
        ifu (ReadConsoleInputW(hdl, &ir, 1, &irRead) == 0)
            rete(ErrVal::CON);
        if (ir.EventType != KEY_EVENT || !ir.Event.KeyEvent.bKeyDown)
            continue; // only keep key down
        cx AU& vkCode = ir.Event.KeyEvent.wVirtualKeyCode;
        switch (vkCode)
        {
        case VK_RETURN: {
            if (strLen > 0)
            {
                if (historyPos != -1)
                    s_history->pop_back();
                if (s_history->size() == 0 || StrCmp(str, (*s_history)[s_history->size() - 1].c_str()) != 0)
                {
                    if (s_history->size() == CON_HISTORY_CNT_MAX)
                        s_history->erase(s_history->begin());
                    s_history->push_back(str);
                }
            }
            ife (_ConWriteRaw("\n", 1, YES))
                retep;
            rets;
        }
        case VK_ESCAPE: {
            if (strLen < 1)
                break;
            if (isShow)
            {
                cx AU writeLen = strLen;
                std::vector<CS> buf(writeLen);
                MemSet(buf.data(), '\b', strPos * siz(CS));
                ife (_ConWriteRaw(buf.data(), strPos, YES))
                    retep;
                MemSet(buf.data(), ' ', buf.size() * siz(CS));
                ife (_ConWriteRaw(buf.data(), buf.size(), YES))
                    retep;
                MemSet(buf.data(), '\b', buf.size() * siz(CS));
                ife (_ConWriteRaw(buf.data(), buf.size(), YES))
                    retep;
            }
            strPos = 0;
            strLen = 0;
            str[strLen] = '\0';
            break;
        }
        case VK_BACK: {
            if (strPos < 1)
                break;
            --strPos;
            strLen = StrRemove(str, strPos, 1);
            if (isShow)
            {
                ife (_ConCurShow(NO))
                    retep;
                ife (_ConWriteRaw("\b", 1, YES))
                    retep;
                cx AU writeLen = strLen - strPos;
                ife (_ConWriteRaw(str + strPos, writeLen, YES))
                    retep;
                ife (_ConWriteRaw(" ", 1, YES))
                    retep;
                std::vector<CS> buf(writeLen + 1);
                MemSet(buf.data(), '\b', buf.size() * siz(CS));
                ife (_ConWriteRaw(buf.data(), buf.size(), YES))
                    retep;
                ife (_ConCurShow(YES))
                    retep;
            }
            break;
        }
        case VK_LEFT: {
            if (strPos > 0)
            {
                --strPos;
                if (isShow)
                    ife (_ConWriteRaw("\b", 1, YES))
                        retep;
            }
            break;
        }
        case VK_RIGHT: {
            if (strPos < strLen)
            {
                ++strPos;
                if (isShow)
                    ife (_ConWriteRaw(str + strPos - 1, 1, YES))
                        retep;
            }
            break;
        }
        case VK_UP:
        case VK_DOWN: {
            if (s_history->size() < 1)
                break;
            if (vkCode == VK_UP)
            {
                if (historyPos == -1)
                {
                    if (s_history->size() == CON_HISTORY_CNT_MAX)
                        s_history->erase(s_history->begin());
                    s_history->push_back(str);
                    historyPos = s_history->size() - 2;
                }
                else if (historyPos > 0)
                {
                    if (historyPos == SI(s_history->size()) - 1)
                    {
                        (*s_history)[historyPos] = str;
                    }
                    --historyPos;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if ((historyPos != -1) && (historyPos < SI(s_history->size()) - 1))
                {
                    ++historyPos;
                }
                else
                {
                    break;
                }
            }
            if (isShow)
            {
                cx AU writeLen = strLen;
                std::vector<CS> buf(writeLen);
                MemSet(buf.data(), '\b', strPos * siz(CS));
                ife (_ConWriteRaw(buf.data(), strPos, YES))
                    retep;
                MemSet(buf.data(), ' ', buf.size() * siz(CS));
                ife (_ConWriteRaw(buf.data(), buf.size(), YES))
                    retep;
                MemSet(buf.data(), '\b', buf.size() * siz(CS));
                ife (_ConWriteRaw(buf.data(), buf.size(), YES))
                    retep;
            }
            cx AU& strHistory = (*s_history)[historyPos];
            strLen = Min<SI>(strHistory.size(), strLenxMax - STR_EX_LEN);
            MemCpy(str, strHistory.c_str(), strLen * siz(CS));
            str[strLen] = '\0';
            strPos = strLen;
            ife (_ConWriteRaw(str, strLen, YES))
                retep;
            break;
        }
        case VK_HOME: {
            if (strPos > 0)
            {
                if (isShow)
                {
                    std::vector<CS> buf(strPos);
                    MemSet(buf.data(), '\b', buf.size() * siz(CS));
                    ife (_ConWriteRaw(buf.data(), buf.size(), YES))
                        retep;
                }
                strPos = 0;
            }
            break;
        }
        case VK_END: {
            if (strPos < strLen)
            {
                if (isShow)
                {
                    cx AU writeLen = strLen - strPos;
                    ife (_ConWriteRaw(str + strPos, writeLen, YES))
                        retep;
                }
                strPos = strLen;
            }
            break;
        }
        default: {
            if (strLen >= strLenxMax - STR_EX_LEN)
                break;
            cx CS c = CHToCS(CH(ir.Event.KeyEvent.uChar.UnicodeChar));
            if (c == '\0')
                break;
            cx CS s[2] = {c, '\0'};
            strLen = StrInsert(str, s, strPos);
            if (isShow)
            {
                ife (_ConCurShow(NO))
                    retep;
                cx AU writeLen = strLen - strPos;
                ife (_ConWriteRaw(str + strPos, writeLen, YES))
                    retep;
                if (writeLen - 1 > 0)
                {
                    std::vector<CS> buf(writeLen - 1);
                    MemSet(buf.data(), '\b', buf.size() * siz(CS));
                    ife (_ConWriteRaw(buf.data(), buf.size(), YES))
                        retep;
                }
                ife (_ConCurShow(YES))
                    retep;
            }
            ++strPos;
            break;
        }
        case VK_TAB:
        case VK_SHIFT:
        case VK_CONTROL:
        case VK_MENU: {
            break;
        }
        }
    }
#else
    rete(ErrVal::NO_SUPPORT);
#endif
}
dfa ER _ConWait(cx CS* prefix, ConWaitAnim anim)
{
#ifdef PROG_SYS_WIN
    ife (_ConInBufClr())
        retep;
    cx AU hdl = _ConInHdl();
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::NO_EXIST);
    if (prefix != NUL)
        ife (_ConWriteRaw(prefix, StrLen(prefix), YES))
            retep;
    INPUT_RECORD ir;
    DWORD irRead;
    Pos2<SI> posBaseAnim;
    if (anim != ConWaitAnim::STATIC)
        ife (_ConCurPosGet(posBaseAnim))
            retep;
    SI animFrame = 0;
    while (YES)
    {
        ++animFrame;
        if (anim != ConWaitAnim::STATIC)
        {
            Pos2<SI> posNow;
            ife (_ConCurPosGet(posNow))
                retep;
            ife (_ConCurShow(NO))
                retep;
            ife (_ConCurPosSet(Pos2<SI>(posBaseAnim.x, posNow.y)))
                retep;
            std::string animStr;
            if (anim == ConWaitAnim::SPINNER)
            {
                cxex cx CS* animDat = "|/-\\";
                CS animBase[] = "[ ]";
                animBase[1] = animDat[(animFrame / 5) % 4];
                animStr = animBase;
            }
            else if (anim == ConWaitAnim::DOTS)
            {
                animStr = std::string((animFrame / 6) % 5, '.');
                animStr += std::string(4 - (animFrame / 6) % 5, ' ');
            }
            ife (_ConWriteRaw(animStr.c_str(), animStr.size(), YES))
                retep;
            ife (_ConCurShow(YES))
                retep;
        }
        ifu (PeekConsoleInputW(hdl, &ir, 1, &irRead) == 0)
            rete(ErrVal::CON);
        if (irRead == 0)
        {
            ThdWait(30);
            continue;
        }
        ifu (ReadConsoleInputW(hdl, &ir, 1, &irRead) == 0)
            rete(ErrVal::CON);
        if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown)
        {
            if (ir.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
            {
                if (anim != ConWaitAnim::STATIC)
                {
                    Pos2<SI> pos;
                    ife (_ConCurPosGet(pos))
                        retep;
                    ife (_ConClrRow(pos.y))
                        retep;
                    pos.x = 0;
                    ife (_ConCurPosSet(pos))
                        retep;
                    if (prefix != NUL)
                        ife (_ConWriteRaw(prefix, StrLen(prefix), YES))
                            retep;
                }
                if (prefix != NUL)
                    ife (_ConWriteRaw("\n", 1, YES))
                        retep;
                break;
            }
        }
    }
    rets;
#else
    rete(ErrVal::NO_SUPPORT);
#endif
}

dfa BO _ConIsInit()
{
    ret (g_conCtx != NUL);
}
dfa ER _ConInit()
{
    ifl (_ConIsInit())
        rets;
    g_conCtx = new ConCtx;
    if (!ConIsExist())
    {
#if defined(PROG_SYS_WIN)
        ifu (AllocConsole() == 0)
            rete(ErrVal::CON);
        ife (Win(Win::SelType::CON).Focus())
            retep;
        g_conCtx->isConOwn = YES;
#else
        rete(ErrVal::NO_SUPPORT);
#endif
    }
    ife (_ConCurShow(NO))
        retep;
    if (!g_conCtx->isConOwn)
        ife (_ConColRealGet(g_conCtx->colInitSave))
            retep;
    ife (_ConColRealSet(g_conCtx->colReal))
        retep;
    ife (_ConInBufClr())
        retep;
#if defined(PROG_SYS_WIN)
    {
        cx AU hdl = _ConOutHdl();
        CONSOLE_FONT_INFOEX font = {};
        font.cbSize = siz(font);
        font.dwFontSize.X = 10;
        font.dwFontSize.Y = 18;
        font.FontWeight = 500;
        StrCpy(font.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(hdl, YES, &font);
    }
#endif
    rets;
}
dfa ER _ConFree()
{
#ifdef PROG_SYS_WIN
    if (!_ConIsInit())
        rets;
    if (!g_conCtx->isConOwn)
    {
        ife (_ConColRealSet(g_conCtx->colInitSave))
            retep;
        ife (_ConCurShow(YES))
            retep;
    }
    if (g_conCtx->isConOwn && ConIsExist())
    {
        ifu (FreeConsole() == 0)
            rete(ErrVal::CON);
        g_conCtx->isConOwn = NO;
    }
    delete g_conCtx;
    rets;
#else
    rete(ErrVal::NO_SUPPORT);
#endif
}

dfa BO ConIsExist()
{
#if defined(PROG_SYS_WIN)
    ret (GetConsoleWindow() != NUL);
#elif defined(PROG_SYS_LINUX)
    ret isatty(fileno(stdout)) != 0;
#elif defined(PROG_SYS_ESP32)
    ret YES; // assume always exist
#else
    ret NO;
#endif
}
dfa ER ConNew()
{
    ret _ConInit();
}
dfa ER ConDel()
{
    ret _ConFree();
}
dfa ER ConReq()
{
    ifl (_ConIsInit())
        rets;
    ret _ConInit();
}

dfa ER ConColSet(ConCol col)
{
    g_conCtx->col = col;
    rets;
}
dfa ER ConColGet(ConCol& col)
{
    col = g_conCtx->col;
    rets;
}
dfa ER ConTitleSet(cx CH* title)
{
#ifdef PROG_SYS_WIN
    ife (ConReq())
        retep;
    ifu (SetConsoleTitleW(title) == 0)
        rete(ErrVal::CON);
    rets;
#else
    rete(ErrVal::NO_SUPPORT);
#endif
}
dfa ER ConTitleGet(std::wstring& title)
{
    title.clear();
#ifdef PROG_SYS_WIN
    ArrSbo<CH, CON_SBO_LENX_MAX> buf;
    jdst(retry);
    cx AU titleLen = SI(GetConsoleTitleW(buf.Dat(), U4(buf.Cnt() - STR_EX_LEN)));
    ifu (titleLen < 1)
        rete(ErrVal::CON);
    ifu (titleLen >= buf.Cnt() - STR_EX_LEN)
    {
        buf.Resize(buf.Cnt() * 4, 0);
        jsrc(retry);
    }
    title.assign(buf.Dat(), titleLen);
    rets;
#else
    rete(ErrVal::NO_SUPPORT);
#endif
}

dfa ER ConWriteRaw(cx CS* form, ...)
{
    AL args;
    AlNew(args, form);
    cx AU err = _ConWriteRawAl(form, args, NUL, NUL);
    AlDel(args);
    ret err;
}
dfa ER ConWrite(cx CS* form, ...)
{
    AL args;
    AlNew(args, form);
    cx AU err = _ConWriteRawAl(form, args, NUL, "\n");
    AlDel(args);
    ret err;
}
dfa ER ConWriteDbg(cx CS* form, ...)
{
    ifndbg (YES)
        rets;
    AL args;
    AlNew(args, form);
    cx AU err = _ConWriteRawAl(form, args, "&7;[DBG] &f;", "\n");
    AlDel(args);
    ret err;
}
dfa ER ConWriteInfo(cx CS* form, ...)
{
    AL args;
    AlNew(args, form);
    cx AU err = _ConWriteRawAl(form, args, "&a;[INFO] &f;", "\n");
    AlDel(args);
    ret err;
}
dfa ER ConWriteWarn(cx CS* form, ...)
{
    AL args;
    AlNew(args, form);
    cx AU err = _ConWriteRawAl(form, args, "&e;[WARNING] &f;", "\n");
    AlDel(args);
    ret err;
}
dfa ER ConWriteErr(cx CS* form, ...)
{
    AL args;
    AlNew(args, form);
    cx AU err = _ConWriteRawAl(form, args, "&c;[ERROR] &f;", "\n");
    AlDel(args);
    ret err;
}

dfa ER ConReadStr(CS* str, SI strLenxMax, SI& strLen, cx CS* prefix, ThdLockFast* prefixLock, BO isShow)
{
    ife (_ConReadModeSet(YES))
        retep;
    cx AU err = _ConReadStr(str, strLenxMax, strLen, prefix, prefixLock, isShow);
    ife (_ConReadModeSet(NO))
        retep;
    ret err;
}
dfa ER ConReadStr(std::string& str, cx CS* prefix, ThdLockFast* prefixLock, BO isShow)
{
    str.clear();
    ArrSbo<CS, CON_SBO_LENX_MAX> buf;
    SI strLen;
    ife (ConReadStr(buf.Dat(), buf.Cnt(), strLen, prefix, prefixLock, isShow))
        retep;
    str.assign(buf.Dat(), strLen);
    rets;
}
dfa ER ConWait(cx CS* prefix, ConWaitAnim anim)
{
    ife (_ConReadModeSet(YES))
        retep;
    cx AU err = _ConWait(prefix, anim);
    ife (_ConReadModeSet(NO))
        retep;
    ret err;
}
dfa ER ConWait()
{
    ife (_ConReadModeSet(YES))
        retep;
    cx AU err = _ConWait(NUL, ConWaitAnim::STATIC);
    ife (_ConReadModeSet(NO))
        retep;
    ret err;
}

dfa ER ConBufClr()
{
#ifdef PROG_SYS_WIN
    if (ConIsExist() == NO)
        rets;
    cx AU hdl = _ConOutHdl();
    ifu (hdl == INVALID_HANDLE_VALUE)
        rete(ErrVal::NO_EXIST);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    ifu (GetConsoleScreenBufferInfo(hdl, &csbi) == 0)
        rete(ErrVal::CON);
    cx DWORD conSize = csbi.dwSize.X * csbi.dwSize.Y;
    cx COORD coord = {0, 0};
    DWORD result = 0;
    ifu (FillConsoleOutputCharacterW(hdl, ' ', conSize, coord, &result) == 0)
        rete(ErrVal::CON);
    ifu (GetConsoleScreenBufferInfo(hdl, &csbi) == 0)
        rete(ErrVal::CON);
    ifu (FillConsoleOutputAttribute(hdl, csbi.wAttributes, conSize, coord, &result) == 0)
        rete(ErrVal::CON);
    ifu (SetConsoleCursorPosition(hdl, coord) == 0)
        rete(ErrVal::CON);
    rets;
#else
    rete(ErrVal::NO_SUPPORT);
#endif
}
