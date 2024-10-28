#pragma once

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
    dfa DictAvlElem& ElemAlloc(cx T1& key, cx T2& val, DictAvlElem* parent) cx
    {
        DictAvlElem& elem = *(new DictAvlElem);
        elem.parent = parent;
        elem.left = NUL;
        elem.right = NUL;
        elem.lv = 1;
        elem.key = key;
        elem.val = val;
        ret elem;
    }
    dfa U1 LvGet(cx DictAvlElem* elem) cx
    {
        ret (elem != NUL) ? elem->lv : 0;
    }
    dfa NT LvUpd(DictAvlElem& elem) cx
    {
        elem.lv = 1 + Max<U1>(tx->LvGet(elem.left), tx->LvGet(elem.right));
    }
    dfa S1 BalGet(cx DictAvlElem& elem) cx
    {
        ret tx->LvGet(elem.left) - tx->LvGet(elem.right);
    }
    dfa NT RotLL(DictAvlElem*& elem) cx
    {
        DictAvlElem& e = *elem;
        if (e.parent != NUL)
        {
            if (e.parent->left == elem)
                e.parent->left = e.left;
            else
                e.parent->right = e.left;
        }
        if (e.left->right != NUL)
            e.left->right->parent = elem;
        e.left->parent = e.parent;
        e.parent = e.left;
        e.left = e.left->right;
        e.parent->right = elem;
        e.lv -= 2;
        elem = e.parent;
    }
    dfa NT RotRR(DictAvlElem*& elem) cx
    {
        DictAvlElem& e = *elem;
        if (e.parent != NUL)
        {
            if (e.parent->right == elem)
                e.parent->right = e.right;
            else
                e.parent->left = e.right;
        }
        if (e.right->left != NUL)
            e.right->left->parent = elem;
        e.right->parent = e.parent;
        e.parent = e.right;
        e.right = e.right->left;
        e.parent->left = elem;
        e.lv -= 2;
        elem = e.parent;
    }
    dfa NT RotLR(DictAvlElem*& elem) cx
    {
        DictAvlElem& e = *elem;
        if (e.parent != NUL)
        {
            if (e.parent->left == elem)
                e.parent->left = e.left->right;
            else
                e.parent->right = e.left->right;
        }
        e.left->right->parent = e.parent;
        e.parent = e.left->right;
        if (e.parent->left != NUL)
            e.parent->left->parent = e.left;
        if (e.parent->right != NUL)
            e.parent->right->parent = elem;
        e.left->parent = e.parent;
        e.left->right = e.parent->left;
        e.parent->left = e.left;
        e.left = e.parent->right;
        e.parent->right = elem;
        elem = e.parent;
        elem->lv += 1;
        elem->left->lv -= 1;
        elem->right->lv -= 2;
    }
    dfa NT RotRL(DictAvlElem*& elem) cx
    {
        DictAvlElem& e = *elem;
        if (e.parent != NUL)
        {
            if (e.parent->left == elem)
                e.parent->left = e.right->left;
            else
                e.parent->right = e.right->left;
        }
        e.right->left->parent = e.parent;
        e.parent = e.right->left;
        if (e.parent->right != NUL)
            e.parent->right->parent = e.right;
        if (e.parent->left != NUL)
            e.parent->left->parent = elem;
        e.right->parent = e.parent;
        e.right->left = e.parent->right;
        e.parent->right = e.right;
        e.right = e.parent->left;
        e.parent->left = elem;
        elem = e.parent;
        elem->lv += 1;
        elem->right->lv -= 1;
        elem->left->lv -= 2;
    }
    dfa DictAvlElem* Bal(DictAvlElem* elem) cx
    {
        cx S1 bal = tx->BalGet(*elem);
        if (bal > 1)
        {
            if (tx->BalGet(*elem->left) >= 0)
                tx->RotLL(elem);
            else
                tx->RotLR(elem);
            ret elem;
        }
        if (bal < -1)
        {
            if (tx->BalGet(*elem->right) <= 0)
                tx->RotRR(elem);
            else
                tx->RotRL(elem);
            ret elem;
        }
        ret NUL;
    }
    dfa NT TravelUp(DictAvlElem* elem)
    {
        while (elem != NUL)
        {
            tx->LvUpd(*elem);
            DictAvlElem* r = tx->Bal(elem);
            if (r != NUL)
            {
                ifu (r->parent == NUL)
                    m_root = r;
                ret;
            }
            elem = elem->parent;
        }
    }
    dfa DictAvlElem* Srch(cx T1& key) cx
    {
        DictAvlElem* elem = m_root;
        while (elem != NUL)
        {
            if (key < elem->key)
                elem = elem->left;
            else ifl (key > elem->key)
                elem = elem->right;
            else
                break;
        }
        ret elem;
    }
    dfa cx DictAvlElem* ElemMin(cx DictAvlElem* elem) cx
    {
        ifu (elem == NUL)
            ret NUL;
        while (elem->left != NUL)
            elem = elem->left;
        ret elem;
    }
    dfa cx DictAvlElem* ElemMax(cx DictAvlElem* elem) cx
    {
        ifu (elem == NUL)
            ret NUL;
        while (elem->right != NUL)
            elem = elem->right;
        ret elem;
    }
    dfa cx DictAvlElem* ElemPrev(cx T1& key) cx
    {
        cx DictAvlElem* elem = tx->Srch(key);
        ifu (elem == NUL)
            ret NUL;
        if (elem->left != NUL)
            ret tx->ElemMax(elem->left);
        while (elem->parent != NUL && elem == elem->parent->left)
            elem = elem->parent;
        ret elem->parent;
    }
    dfa cx DictAvlElem* ElemNext(cx T1& key) cx
    {
        cx DictAvlElem* elem = tx->Srch(key);
        ifu (elem == NUL)
            ret NUL;
        if (elem->right != NUL)
            ret tx->ElemMin(elem->right);
        while (elem->parent != NUL && elem == elem->parent->right)
            elem = elem->parent;
        ret elem->parent;
    }
    dfa NT Del(DictAvlElem* elem)
    {
        cx DictAvlElem* replace;
        if (elem->right)
            replace = tx->ElemMin(elem->right);
        else if (elem->left)
            replace = tx->ElemMax(elem->left);
        else
            replace = elem;
        if (replace->parent)
        {
            if (replace == replace->parent->left)
            {
                if (replace->left)
                {
                    replace->parent->left = replace->left;
                    replace->left->parent = replace->parent;
                }
                else if (replace->right)
                {
                    replace->parent->left = replace->right;
                    replace->right->parent = replace->parent;
                }
                else
                {
                    replace->parent->left = NUL;
                }
            }
            else
            {
                if (replace->left)
                {
                    replace->parent->right = replace->left;
                    replace->left->parent = replace->parent;
                }
                else if (replace->right)
                {
                    replace->parent->right = replace->right;
                    replace->right->parent = replace->parent;
                }
                else
                {
                    replace->parent->right = NUL;
                }
            }
        }
        else
        {
            m_root = NUL;
        }
        elem->key = replace->key;
        elem->val = replace->val;
        if (replace->parent)
        {
            tx->TravelUp(replace->parent);
            tx->LvUpd(*replace->parent);
            if (replace->parent->parent)
                tx->LvUpd(*replace->parent->parent);
        }
        delete replace;
    }
    dfa SI _Len(cx DictAvlElem* elem) cx
    {
        if (elem == NUL)
            ret 0;
        ret 1 + tx->_Len(elem->left) + tx->_Len(elem->right);
    }
    dfa SI _Height(cx DictAvlElem* elem) cx
    {
        if (elem == NUL)
            ret 0;
        ret 1 + Max<SI>(tx->_Height(elem->left), tx->_Height(elem->right));
    }
    dfa NT _Clr(DictAvlElem* elem) cx
    {
        if (elem == NUL)
            ret;
        tx->_Clr(elem->left);
        tx->_Clr(elem->right);
        delete elem;
    }

  public:
    dfa BO Add(cx T1& key, cx T2& val)
    {
        DictAvlElem* parent = NUL;
        DictAvlElem** elem = &m_root;
        while (*elem != NUL)
        {
            parent = *elem;
            if (key < (*elem)->key)
                elem = &((*elem)->left);
            else if (key > (*elem)->key)
                elem = &((*elem)->right);
            else
                ret NO;
        }
        *elem = &tx->ElemAlloc(key, val, parent);
        tx->TravelUp(*elem);
        ret YES;
    }
    dfa BO Add(cx Keyval<T1, T2>& keyval)
    {
        ret tx->Add(keyval.key, keyval.val);
    }
    dfa BO Replace(cx T1& key, cx T2& val)
    {
        DictAvlElem* parent = NUL;
        DictAvlElem** elem = &m_root;
        while (*elem != NUL)
        {
            parent = *elem;
            if (key < (*elem)->key)
                elem = &((*elem)->left);
            else if (key > (*elem)->key)
                elem = &((*elem)->right);
            else
            {
                (*elem)->val = val;
                ret YES;
            }
        }
        *elem = &tx->ElemAlloc(key, val, parent);
        tx->TravelUp(*elem);
        ret NO;
    }
    dfa BO Replace(cx Keyval<T1, T2>& keyval)
    {
        ret tx->Replace(keyval.key, keyval.val);
    }
    dfa BO Del(cx T1& key)
    {
        DictAvlElem* elem = tx->Srch(key);
        ifu (elem == NUL)
            ret NO;
        tx->Del(elem);
        ret YES;
    }
    dfa T2* Get(cx T1& key) cx
    {
        DictAvlElem* elem = tx->Srch(key);
        ifu (elem == NUL)
            ret NUL;
        ret &(elem->val);
    }
    dfa SI Len() cx
    {
        ret tx->_Len(m_root);
    }
    dfa SI Height() cx
    {
        ret tx->_Height(m_root);
    }
    dfa NT Clr()
    {
        tx->_Clr(m_root);
        m_root = NUL;
    }
    dfa DictAvl()
    {
        m_root = NUL;
    }
    dfa BO operator+=(cx Keyval<T1, T2>& keyval)
    {
        ret tx->Add(keyval);
    }
    dfa BO operator-=(cx T1& key)
    {
        ret tx->Del(key);
    }
    dfa T2* operator[](cx T1& key) cx
    {
        ret tx->Get(key);
    }
};

class SVarBlock
{
  private:
    DictAvl<string, string> m_vars;

  public:
    dfa NT Clr()
    {
        m_vars.Clr();
    }
    dfa NT Set(cx string& var, cx string& val)
    {
        m_vars.Replace(var, val);
    }
    dfa string Get(cx string& var) cx
    {
        cx string* cx val = m_vars[var];
        if (val == NUL)
            ret string();
        ret *val;
    }
    dfa SI LoadMemCfg(DatIte<U1> dat)
    {
        DatIte<U1> dat_ = dat;
        Arr<CS> buf;
        Arr<CS> buf2;
        Arr<CS> hdr;
        SI hdrLen = 0;
        while (dat_.HasNextLine())
        {
            buf.Req(dat_.NextLineLen() + 1);
            cx SI readLen = dat_.ReadLine((U1*)buf.Ptr());
            buf[readLen] = '\0';
            CS* p = (CS*)StrFind(buf.Ptr(), '=');
            if (p == NUL)
            {
                p = (CS*)StrFind(buf.Ptr(), '[');
                if (p == NUL)
                    continue;
                CS* p2 = (CS*)StrFind(p, ']');
                if (p2 == NUL)
                    continue;
                ++p;
                *p2 = '\0';
                hdrLen = StrTrimWspace(p);
                hdr.Req(hdrLen + 1);
                MemCpy(hdr.Ptr(), p, (hdrLen + 1) * siz(CS));
            }
            else
            {
                *p = '\0';
                CS* var = buf.Ptr();
                cx SI varLen = StrTrimWspace(var);
                if (var[0] == '\0' || var[0] == '#' || var[0] == ';')
                    continue;
                CS* val = p + 1;
                StrTrimWspace(val);
                if (hdrLen == 0)
                {
                    tx->Set(var, val);
                }
                else
                {
                    buf2.Req(hdrLen + 1 + varLen + 1);
                    MemCpy(buf2.Ptr(), hdr.Ptr(), hdrLen * siz(CS));
                    buf2[hdrLen] = '.';
                    MemCpy(buf2.Ptr() + hdrLen + 1, var, (varLen + 1) * siz(CS));
                    tx->Set(buf2.Ptr(), val);
                }
            }
        }
        ret dat_.Pos() - dat.Pos();
    }
    dfa ER LoadFileCfg(cx CH* path)
    {
        Arr<U1> fileDat;
        ife (FileToArr(fileDat, path))
            retep;
        cx SI len = tx->LoadMemCfg(fileDat.ToDatIte());
        unused(len);
        rets;
    }

  public:
    SVarBlock()
    {
    }
    ~SVarBlock()
    {
    }
};
