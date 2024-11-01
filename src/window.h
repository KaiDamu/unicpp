#pragma once

class Win
{
  private:
    HWND m_hdl;

  public:
    enum class SelType : U1
    {
        NONE = 0,
        FOCUS = 1,
        TITLE_CLASS = 2,
        CON = 3,
    };

  public:
    dfa ER Sel(SelType selType, CXGA param1 = NUL, CXGA param2 = NUL)
    {
        switch (selType)
        {
        case SelType::NONE: {
            m_hdl = NUL;
            rets;
        }
        case SelType::FOCUS: {
            m_hdl = GetForegroundWindow();
            ifu (m_hdl == NUL)
                rete(ERR_WIN);
            rets;
        }
        case SelType::TITLE_CLASS: {
            cx AU titleStr = (cx CH*)param1;
            cx AU classStr = (cx CH*)param2;
            m_hdl = FindWindowW(classStr, titleStr);
            ifu (m_hdl == NUL)
                rete(ERR_WIN);
            rets;
        }
        case SelType::CON: {
            m_hdl = GetConsoleWindow();
            ifu (m_hdl == NUL)
                rete(ERR_WIN);
            rets;
        }
        default: {
            rete(ERR_NO_SUPPORT);
        }
        }
    }
    dfa wstring TitleStrGet()
    {
        cx AU strLen = SI(GetWindowTextLengthW(m_hdl));
        if (strLen == 0)
            ret wstring(L"");
        cx AU strLenx = strLen + STR_EX_LEN;
        vector<CH> str(strLenx);
        ifu (GetWindowTextW(m_hdl, str.data(), strLenx) == 0)
            ret wstring(L"");
        ret wstring(str.data());
    }
    dfa ER TitleStrSet(cx CH* str)
    {
        ifu (SetWindowTextW(m_hdl, str) == 0)
            rete(ERR_WIN);
        rets;
    }
    dfa wstring ClassStrGet()
    {
        cx AU strLen = SI(GetClassNameW(m_hdl, NUL, 0));
        if (strLen == 0)
            ret wstring(L"");
        cx AU strLenx = strLen + STR_EX_LEN;
        vector<CH> str(strLenx);
        ifu (GetClassNameW(m_hdl, str.data(), strLenx) == 0)
            ret wstring(L"");
        ret wstring(str.data());
    }
    dfa BO IsValid()
    {
        ret (m_hdl != NUL) && (IsWindow(m_hdl) != 0);
    }
    dfa ER Focus()
    {
        ifu (SetForegroundWindow(m_hdl) == 0)
            rete(ERR_WIN);
        rets;
    }

  public:
    dfa Win(SelType selType = SelType::NONE, CXGA param1 = NUL, CXGA param2 = NUL) : m_hdl(NUL)
    {
        tx->Sel(selType, param1, param2);
    }
    dfa ~Win()
    {
    }
};
