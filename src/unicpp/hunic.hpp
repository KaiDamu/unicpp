#pragma once

#include "container.hpp"

using HunicCh = U1;

cxex SI HUNIC_CH_CNT = 31;

dfa ER HunicChLines(std::vector<Line2<F4>>& lines, HunicCh ch, F4 scale);

dfa NT StrToHunicStr(std::vector<HunicCh>& out, cx CH* in);

#include "hunic.ipp"
