#include "dictionary.hpp"

tpl1 dfa ER SVarBlock::Get(T1& val, cx CS* var, cx T1& defa) cx
{
    std::string str;
    ife (tx->_Get(str, var))
    {
        val = defa;
        retep;
    }
    ToType<T1>(val, str);
    rets;
}

tpl2 dfa DictAvl<T1, T2>::DictAvlElem& DictAvl<T1, T2>::ElemAlloc(cx T1& key, cx T2& val, DictAvlElem* parent) cx
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
tpl2 dfa U1 DictAvl<T1, T2>::LvGet(cx DictAvlElem* elem) cx
{
    ret (elem != NUL) ? elem->lv : 0;
}
tpl2 dfa NT DictAvl<T1, T2>::LvUpd(DictAvlElem& elem) cx
{
    elem.lv = 1 + Max<U1>(tx->LvGet(elem.left), tx->LvGet(elem.right));
}
tpl2 dfa S1 DictAvl<T1, T2>::BalGet(cx DictAvlElem& elem) cx
{
    ret S1(tx->LvGet(elem.left) - tx->LvGet(elem.right));
}
tpl2 dfa NT DictAvl<T1, T2>::RotLL(DictAvlElem*& elem) cx
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
tpl2 dfa NT DictAvl<T1, T2>::RotRR(DictAvlElem*& elem) cx
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
tpl2 dfa NT DictAvl<T1, T2>::RotLR(DictAvlElem*& elem) cx
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
tpl2 dfa NT DictAvl<T1, T2>::RotRL(DictAvlElem*& elem) cx
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
tpl2 dfa DictAvl<T1, T2>::DictAvlElem* DictAvl<T1, T2>::Bal(DictAvlElem* elem) cx
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
tpl2 dfa NT DictAvl<T1, T2>::TravelUp(DictAvlElem* elem)
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
tpl2 dfa DictAvl<T1, T2>::DictAvlElem* DictAvl<T1, T2>::Srch(cx T1& key) cx
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
tpl2 dfa cx DictAvl<T1, T2>::DictAvlElem* DictAvl<T1, T2>::ElemMin(cx DictAvlElem* elem) cx
{
    ifu (elem == NUL)
        ret NUL;
    while (elem->left != NUL)
        elem = elem->left;
    ret elem;
}
tpl2 dfa cx DictAvl<T1, T2>::DictAvlElem* DictAvl<T1, T2>::ElemMax(cx DictAvlElem* elem) cx
{
    ifu (elem == NUL)
        ret NUL;
    while (elem->right != NUL)
        elem = elem->right;
    ret elem;
}
tpl2 dfa cx DictAvl<T1, T2>::DictAvlElem* DictAvl<T1, T2>::ElemPrev(cx T1& key) cx
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
tpl2 dfa cx DictAvl<T1, T2>::DictAvlElem* DictAvl<T1, T2>::ElemNext(cx T1& key) cx
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
tpl2 dfa NT DictAvl<T1, T2>::Del(DictAvlElem* elem)
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
tpl2 dfa SI DictAvl<T1, T2>::_Len(cx DictAvlElem* elem) cx
{
    if (elem == NUL)
        ret 0;
    ret 1 + tx->_Len(elem->left) + tx->_Len(elem->right);
}
tpl2 dfa SI DictAvl<T1, T2>::_Height(cx DictAvlElem* elem) cx
{
    if (elem == NUL)
        ret 0;
    ret 1 + Max<SI>(tx->_Height(elem->left), tx->_Height(elem->right));
}
tpl2 dfa NT DictAvl<T1, T2>::_Clr(DictAvlElem* elem) cx
{
    if (elem == NUL)
        ret;
    tx->_Clr(elem->left);
    tx->_Clr(elem->right);
    delete elem;
}
tpl2 dfa BO DictAvl<T1, T2>::Add(cx T1& key, cx T2& val)
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
tpl2 dfa BO DictAvl<T1, T2>::Add(cx Keyval<T1, T2>& keyval)
{
    ret tx->Add(keyval.key, keyval.val);
}
tpl2 dfa BO DictAvl<T1, T2>::Replace(cx T1& key, cx T2& val)
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
tpl2 dfa BO DictAvl<T1, T2>::Replace(cx Keyval<T1, T2>& keyval)
{
    ret tx->Replace(keyval.key, keyval.val);
}
tpl2 dfa BO DictAvl<T1, T2>::Del(cx T1& key)
{
    DictAvlElem* elem = tx->Srch(key);
    ifu (elem == NUL)
        ret NO;
    tx->Del(elem);
    ret YES;
}
tpl2 dfa T2* DictAvl<T1, T2>::Get(cx T1& key) cx
{
    DictAvlElem* elem = tx->Srch(key);
    ifu (elem == NUL)
        ret NUL;
    ret &(elem->val);
}
tpl2 dfa SI DictAvl<T1, T2>::Len() cx
{
    ret tx->_Len(m_root);
}
tpl2 dfa SI DictAvl<T1, T2>::Height() cx
{
    ret tx->_Height(m_root);
}
tpl2 dfa NT DictAvl<T1, T2>::Clr()
{
    tx->_Clr(m_root);
    m_root = NUL;
}
tpl2 dfa DictAvl<T1, T2>::DictAvl()
{
    m_root = NUL;
}
tpl2 dfa BO DictAvl<T1, T2>::operator+=(cx Keyval<T1, T2>& keyval)
{
    ret tx->Add(keyval);
}
tpl2 dfa BO DictAvl<T1, T2>::operator-=(cx T1& key)
{
    ret tx->Del(key);
}
tpl2 dfa T2* DictAvl<T1, T2>::operator[](cx T1& key) cx
{
    ret tx->Get(key);
}
