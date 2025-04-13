#pragma once

using HunicCh = U1;

cxex SI HUNIC_CH_CNT = 31;

dfa ER HunicChLines(std::vector<Line2<F4>>& lines, HunicCh ch, F4 scale)
{
    lines.clear();
    ifu (ch >= HUNIC_CH_CNT)
        rete(ErrVal::NO_EXIST);
    cx F4 posDat[487] = {-1.0f, -1.0f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.50f, 0.50f, 0.50f, 0.00f, 0.50f, 0.00f, 1.00f, 0.00f, 0.00f, 0.25f, 0.25f, 0.50f, 0.50f, 0.25f, 0.25f, -1.0f, 0.00f, 0.00f,
                         0.00f, 0.50f, 0.50f, 0.00f, 0.50f, 0.50f, 0.00f, 0.50f, 0.00f, 1.00f, 0.00f, 0.00f, 0.25f, 0.25f, 0.50f, 0.50f, 0.25f, 0.25f, -1.0f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.50f,
                         0.00f, 1.00f, 0.00f, 0.50f, 0.50f, 0.75f, 0.00f, 1.00f, 0.50f, 0.75f, -1.0f, 0.50f, 0.00f, 0.25f, 0.25f, 0.00f, 0.50f, 0.25f, 0.25f, 0.00f, 0.50f, 0.25f, 0.75f, 0.50f, 1.00f,
                         0.25f, 0.75f, -1.0f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.50f, 0.00f, 1.00f, 0.00f, 0.00f, 0.25f, 0.25f, 0.50f, 0.50f, 0.25f, 0.25f, 0.50f, 0.50f, 0.25f, 0.75f, 0.00f, 1.00f,
                         0.25f, 0.75f, -1.0f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.50f, 0.00f, 1.00f, -1.0f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 0.00f, 0.50f, 0.50f,
                         0.00f, 0.50f, 0.50f, 0.00f, 0.50f, 0.00f, 1.00f, -1.0f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.50f, 0.50f, 0.50f, 0.00f, 0.50f, 0.00f, 1.00f, -1.0f,
                         0.50f, 0.50f, 0.50f, 1.00f, 0.50f, 0.00f, 0.25f, 0.25f, 0.00f, 0.50f, 0.25f, 0.25f, 0.00f, 0.50f, 0.25f, 0.75f, 0.50f, 1.00f, 0.25f, 0.75f, -1.0f, 0.50f, 0.00f, 0.50f, 0.50f,
                         0.00f, 0.50f, 0.50f, 0.50f, 0.50f, 0.50f, 0.50f, 1.00f, -1.0f, 0.00f, 0.00f, 0.50f, 0.00f, 0.50f, 0.00f, 0.50f, 0.50f, 0.50f, 0.50f, 0.50f, 1.00f, 0.00f, 1.00f, 0.50f, 1.00f,
                         -1.0f, 0.50f, 0.00f, 0.50f, 0.50f, 0.50f, 0.50f, 0.50f, 1.00f, 0.00f, 0.50f, 0.25f, 0.75f, 0.50f, 1.00f, 0.25f, 0.75f, -1.0f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.50f, 0.00f,
                         1.00f, 0.00f, 1.00f, 0.50f, 1.00f, 0.50f, 0.50f, 0.25f, 0.75f, 0.00f, 1.00f, 0.25f, 0.75f, -1.0f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.50f, 0.00f, 1.00f, 0.00f, 1.00f, 0.50f,
                         1.00f, -1.0f, 0.50f, 0.00f, 0.50f, 0.50f, 0.00f, 0.50f, 0.00f, 1.00f, 0.50f, 0.50f, 0.50f, 1.00f, 0.50f, 0.00f, 0.25f, 0.25f, 0.00f, 0.50f, 0.25f, 0.25f, -1.0f, 0.00f, 0.00f,
                         0.00f, 0.50f, 0.00f, 0.50f, 0.00f, 1.00f, 0.50f, 0.50f, 0.50f, 1.00f, 0.00f, 0.00f, 0.25f, 0.25f, 0.50f, 0.50f, 0.25f, 0.25f, -1.0f, 0.00f, 1.00f, 0.50f, 1.00f, 0.00f, 1.00f,
                         0.25f, 0.00f, 0.50f, 1.00f, 0.25f, 0.00f, -1.0f, 0.00f, 1.00f, 0.50f, 1.00f, 0.00f, 0.00f, 0.25f, 0.50f, 0.50f, 0.00f, 0.25f, 0.50f, 0.00f, 1.00f, 0.25f, 0.50f, 0.50f, 1.00f,
                         0.25f, 0.50f, -1.0f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.50f, 0.00f, 1.00f, 0.00f, 0.00f, 0.50f, 0.25f, 0.00f, 0.50f, 0.50f, 0.25f, -1.0f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f,
                         0.00f, 0.00f, 0.50f, 0.00f, 0.50f, 0.00f, 1.00f, 0.00f, 0.50f, 0.25f, 0.75f, 0.50f, 1.00f, 0.25f, 0.75f, -1.0f, 0.50f, 0.00f, 0.50f, 0.50f, 0.00f, 0.50f, 0.50f, 0.50f, 0.00f,
                         0.50f, 0.00f, 1.00f, -1.0f, 0.50f, 0.00f, 0.50f, 0.50f, 0.00f, 0.50f, 0.50f, 0.50f, 0.00f, 0.50f, 0.00f, 1.00f, 0.50f, 0.50f, 0.50f, 1.00f, -1.0f, 0.00f, 0.00f, 0.50f, 0.00f,
                         0.50f, 0.00f, 0.50f, 0.50f, 0.50f, 0.50f, 0.50f, 1.00f, -1.0f, 0.00f, 0.00f, 0.00f, 1.00f, 0.50f, 0.00f, 0.50f, 1.00f, 0.00f, 1.00f, 0.50f, 1.00f, -1.0f, 0.00f, 0.00f, 0.50f,
                         0.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.50f, 0.00f, 0.50f, 1.00f, -1.0f, 0.00f, 0.00f, 0.25f, 1.00f, 0.50f, 0.00f, 0.25f, 1.00f, -1.0f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 1.00f,
                         0.50f, 1.00f, 0.50f, 0.00f, 0.25f, 0.50f, 0.00f, 1.00f, 0.25f, 0.50f, -1.0f, 0.00f, 0.00f, 0.50f, 0.00f, 0.50f, 0.00f, 0.50f, 1.00f, 0.50f, 0.00f, 0.25f, 0.50f, 0.00f, 1.00f,
                         0.25f, 0.50f, -1.0f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 1.00f, 0.50f, 1.00f, -1.0f};
    cx SI posDatI[HUNIC_CH_CNT] = {0, 1, 2, 23, 44, 61, 78, 103, 116, 133, 150, 171, 184, 201, 218, 239, 252, 273, 294, 307, 328, 345, 366, 379, 396, 409, 422, 435, 444, 461, 478};
    AU p = posDat + posDatI[SI(ch)];
    while (*p != -1.0f)
    {
        lines.emplace_back(Pos2<F4>(p[0], p[1]) * scale, Pos2<F4>(p[2], p[3]) * scale);
        p += 4;
    }
    rets;
}

dfa NT StrToHunicStr(std::vector<HunicCh>& out, cx CH* in)
{
    out.clear();
    cx AU inLenx = StrLen(in) + STR_EX_LEN;
    std::vector<CH> tmp1(inLenx * 2);
    std::vector<CH> tmp2(inLenx * 2);

    MemCpy(tmp1.data(), in, inLenx * siz(CH));
    StrToLowcaseHun(tmp1.data());

    cx CH* replaceList[7] = {L"j", L"j", L"ii", L"\xFC\xFC", L"\xF6\xF6", L"uu", L"oo"};
    cx CH* findList[7] = {L"ly", L"y", L"\xED", L"\x171", L"\x151", L"\xFA", L"\xF3"};
    StrFindReplaceMulti(tmp2.data(), tmp1.data(), std::span<cx CH*>(replaceList, 7), std::span<cx CH*>(findList, 7));

    CH* c = tmp2.data();
    while (*c != L'\0')
    {
        HunicCh hunicCh = 0;
        // clang-format off
        switch (*c)
        {
        case L' ': hunicCh = 1; break;
        case L'a': hunicCh = 2; break;
        case L'\xE1': hunicCh = 3; break;
        case L'b': hunicCh = 4; break;
        case L'c': if (*(c + 1) == L's') hunicCh = 24; else hunicCh = 5; break;
        case L'd': hunicCh = 6; break;
        case L'e': hunicCh = 7; break;
        case L'\xE9': hunicCh = 8; break;
        case L'f': hunicCh = 9; break;
        case L'g': hunicCh = 10; break;
        case L'h': hunicCh = 11; break;
        case L'i': hunicCh = 12; break;
        case L'j': hunicCh = 13; break;
        case L'k': hunicCh = 14; break;
        case L'l': hunicCh = 15; break;
        case L'm': hunicCh = 16; break;
        case L'n': hunicCh = 17; break;
        case L'o': hunicCh = 18; break;
        case L'\xF6': hunicCh = 19; break;
        case L'p': hunicCh = 20; break;
        case L'r': hunicCh = 21; break;
        case L's': if (*(c + 1) == L'z') { hunicCh = 23; ++c; } else { hunicCh = 22; } break;
        case L't': hunicCh = 24; break;
        case L'u': hunicCh = 25; break;
        case L'\xFC': hunicCh = 26; break;
        case L'v': hunicCh = 27; break;
        case L'w': hunicCh = 27; break;
        case L'z': if (*(c + 1) == L's') { hunicCh = 29; ++c; } else { hunicCh = 28; } break;
        default: hunicCh = 1; break;
        }
        // clang-format on
        if (out.size() > 0 && out.back() == hunicCh && out.back() != 1)
            out.emplace_back(30);
        else
            out.emplace_back(hunicCh);
        ++c;
    }
    out.emplace_back(0);
}
