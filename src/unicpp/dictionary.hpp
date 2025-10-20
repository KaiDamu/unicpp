#pragma once

#include "string.hpp"
#include "array.hpp"
#include "tuple.hpp"

class SVarBlock
{
  private:
    std::unordered_map<FNV1A64, std::string, StdHasherNoOpe<FNV1A64>> m_vars;

  private:
    dfa ER _Get(std::string& val, cx CS* var) cx;

  public:
    dfa NT Clr();
    dfa NT Set(cx CS* var, cx std::string& val);
    dfa std::string Get(cx CS* var) cx;
    tpl1 dfa ER Get(T1& val, cx CS* var, cx T1& defa = T1()) cx;
    dfa SI LoadMemCfg(DatIte<U1> dat);
    dfa ER LoadFileCfg(cx CH* path);

  public:
    dfa SVarBlock();
    dfa ~SVarBlock();
};

tpl2 class DictAvl
{
  private:
    struct DictAvlElem
    {
        DictAvlElem* parent;
        DictAvlElem* left;
        DictAvlElem* right;
        U1 lv;
        T1 key;
        T2 val;
    };

  private:
    DictAvlElem* m_root;

  private:
    dfa DictAvlElem& ElemAlloc(cx T1& key, cx T2& val, DictAvlElem* parent) cx;
    dfa U1 LvGet(cx DictAvlElem* elem) cx;
    dfa NT LvUpd(DictAvlElem& elem) cx;
    dfa S1 BalGet(cx DictAvlElem& elem) cx;
    dfa NT RotLL(DictAvlElem*& elem) cx;
    dfa NT RotRR(DictAvlElem*& elem) cx;
    dfa NT RotLR(DictAvlElem*& elem) cx;
    dfa NT RotRL(DictAvlElem*& elem) cx;
    dfa DictAvlElem* Bal(DictAvlElem* elem) cx;
    dfa NT TravelUp(DictAvlElem* elem);
    dfa DictAvlElem* Srch(cx T1& key) cx;
    dfa cx DictAvlElem* ElemMin(cx DictAvlElem* elem) cx;
    dfa cx DictAvlElem* ElemMax(cx DictAvlElem* elem) cx;
    dfa cx DictAvlElem* ElemPrev(cx T1& key) cx;
    dfa cx DictAvlElem* ElemNext(cx T1& key) cx;
    dfa NT Del(DictAvlElem* elem);
    dfa SI _Len(cx DictAvlElem* elem) cx;
    dfa SI _Height(cx DictAvlElem* elem) cx;
    dfa NT _Clr(DictAvlElem* elem) cx;

  public:
    dfa BO Add(cx T1& key, cx T2& val);
    dfa BO Add(cx Keyval<T1, T2>& keyval);
    dfa BO Replace(cx T1& key, cx T2& val);
    dfa BO Replace(cx Keyval<T1, T2>& keyval);
    dfa BO Del(cx T1& key);
    dfa T2* Get(cx T1& key) cx;
    dfa SI Len() cx;
    dfa SI Height() cx;
    dfa NT Clr();
    dfa DictAvl();
    dfa BO operator+=(cx Keyval<T1, T2>& keyval);
    dfa BO operator-=(cx T1& key);
    dfa T2* operator[](cx T1& key) cx;
};

#include "dictionary.ipp"
