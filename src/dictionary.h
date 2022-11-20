#pragma once

tpl2 class DictAvl {
private:
	struct DictAvlElem {
		DictAvlElem* parent;
		DictAvlElem* left;
		DictAvlElem* right;
		U1 lv;
		Keyval<T1, T2> keyval;
	};
private:
	DictAvlElem* m_root;
private:
	dfa DictAvlElem& ElemAlloc(cx Keyval<T1, T2>& keyval, DictAvlElem* parent) const {
		DictAvlElem& elem = *(new DictAvlElem);
		elem.parent = parent;
		elem.left = NUL;
		elem.right = NUL;
		elem.lv = 1;
		elem.keyval = keyval;
		ret elem;
	}
	dfa U1 LvGet(cx DictAvlElem* elem) const {
		ret (elem != NUL) ? elem->lv : 0;
	}
	dfa NT LvUpd(DictAvlElem& elem) const {
		elem.lv = 1 + Max<U1>(tx->LvGet(elem.left), tx->LvGet(elem.right));
	}
	dfa S1 BalGet(cx DictAvlElem& elem) const {
		ret tx->LvGet(elem.left) - tx->LvGet(elem.right);
	}
	dfa NT RotLL(DictAvlElem*& elem) const {
		DictAvlElem& e = *elem;
		if (e.parent != NUL) {
			if (e.parent->left == elem) e.parent->left = e.left;
			else e.parent->right = e.left;
		}
		if (e.left->right != NUL) e.left->right->parent = elem;
		e.left->parent = e.parent;
		e.parent = e.left;
		e.left = e.left->right;
		e.parent->right = elem;
		e.lv -= 2;
		elem = e.parent;
	}
	dfa NT RotRR(DictAvlElem*& elem) const {
		DictAvlElem& e = *elem;
		if (e.parent != NUL) {
			if (e.parent->right == elem) e.parent->right = e.right;
			else e.parent->left = e.right;
		}
		if (e.right->left != NUL) e.right->left->parent = elem;
		e.right->parent = e.parent;
		e.parent = e.right;
		e.right = e.right->left;
		e.parent->left = elem;
		e.lv -= 2;
		elem = e.parent;
	}
	dfa NT RotLR(DictAvlElem*& elem) const {
		DictAvlElem& e = *elem;
		if (e.parent != NUL) {
			if (e.parent->left == elem) e.parent->left = e.left->right;
			else e.parent->right = e.left->right;
		}
		e.left->right->parent = e.parent;
		e.parent = e.left->right;
		if (e.parent->left != NUL) e.parent->left->parent = e.left;
		if (e.parent->right != NUL) e.parent->right->parent = elem;
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
	dfa NT RotRL(DictAvlElem*& elem) const {
		DictAvlElem& e = *elem;
		if (e.parent != NUL) {
			if (e.parent->left == elem) e.parent->left = e.right->left;
			else e.parent->right = e.right->left;
		}
		e.right->left->parent = e.parent;
		e.parent = e.right->left;
		if (e.parent->right != NUL) e.parent->right->parent = e.right;
		if (e.parent->left != NUL) e.parent->left->parent = elem;
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
	dfa DictAvlElem* Bal(DictAvlElem* elem) const {
		cx S1 bal = tx->BalGet(*elem);
		if (bal > 1) {
			if (tx->BalGet(*elem->left) >= 0) tx->RotLL(elem);
			else tx->RotLR(elem);
			ret elem;
		}
		if (bal < -1) {
			if (tx->BalGet(*elem->right) <= 0) tx->RotRR(elem);
			else tx->RotRL(elem);
			ret elem;
		}
		ret NUL;
	}
	dfa NT TravelUp(DictAvlElem* elem) {
		while (elem != NUL) {
			tx->LvUpd(*elem);
			DictAvlElem* r = tx->Bal(elem);
			if (r != NUL) {
				ifu (r->parent == NUL) m_root = r;
				ret;
			}
			elem = elem->parent;
		}
	}
	dfa DictAvlElem* Srch(cx T1& key) const {
		DictAvlElem* elem = m_root;
		while (elem != NUL) {
			if (key < elem->keyval.Key()) elem = elem->left;
			else ifl (key > elem->keyval.Key()) elem = elem->right;
			else break;
		}
		ret elem;
	}
	dfa cx DictAvlElem* ElemMin(cx DictAvlElem* elem) const {
		ifu (elem == NUL) ret NUL;
		while (elem->left != NUL) elem = elem->left;
		ret elem;
	}
	dfa cx DictAvlElem* ElemMax(cx DictAvlElem* elem) const {
		ifu (elem == NUL) ret NUL;
		while (elem->right != NUL) elem = elem->right;
		ret elem;
	}
	dfa cx DictAvlElem* ElemPrev(cx T1& key) const {
		cx DictAvlElem* elem = tx->Srch(key);
		ifu (elem == NUL) ret NUL;
		if (elem->left != NUL) ret tx->ElemMax(elem->left);
		while (elem->parent != NUL && elem == elem->parent->left) elem = elem->parent;
		ret elem->parent;
	}
	dfa cx DictAvlElem* ElemNext(cx T1& key) const {
		cx DictAvlElem* elem = tx->Srch(key);
		ifu (elem == NUL) ret NUL;
		if (elem->right != NUL) ret tx->ElemMin(elem->right);
		while (elem->parent != NUL && elem == elem->parent->right) elem = elem->parent;
		ret elem->parent;
	}
	dfa NT Del(DictAvlElem* elem) {
		cx DictAvlElem* replace;
		if (elem->right) replace = tx->ElemMin(elem->right);
		else if (elem->left) replace = tx->ElemMax(elem->left);
		else replace = elem;
		if (replace->parent) {
			if (replace == replace->parent->left) {
				if (replace->left) {
					replace->parent->left = replace->left;
					replace->left->parent = replace->parent;
				} else if (replace->right) {
					replace->parent->left = replace->right;
					replace->right->parent = replace->parent;
				} else {
					replace->parent->left = NUL;
				}
			} else {
				if (replace->left) {
					replace->parent->right = replace->left;
					replace->left->parent = replace->parent;
				} else if (replace->right) {
					replace->parent->right = replace->right;
					replace->right->parent = replace->parent;
				} else {
					replace->parent->right = NUL;
				}
			}
		} else {
			m_root = NUL;
		}
		elem->keyval = replace->keyval;
		if (replace->parent) {
			tx->TravelUp(replace->parent);
			tx->LvUpd(*replace->parent);
			if (replace->parent->parent) tx->LvUpd(*replace->parent->parent);
		}
		delete replace;
	}
	dfa SI _Len(cx DictAvlElem* elem) {
		if (elem == NUL) ret 0;
		ret 1 + tx->_Len(elem->left) + tx->_Len(elem->right);
	}
	dfa SI _Height(cx DictAvlElem* elem) {
		if (elem == NUL) ret 0;
		ret 1 + Max<SI>(tx->_Height(elem->left), tx->_Height(elem->right));
	}
	dfa NT _Free(DictAvlElem* elem) const {
		if (elem == NUL) ret;
		tx->_Free(elem->left);
		tx->_Free(elem->right);
		delete elem;
	}
public:
	dfa BO Add(cx Keyval<T1, T2>& keyval) {
		DictAvlElem* parent = NUL;
		DictAvlElem** elem = &m_root;
		cx T1& key = keyval.Key();
		while (*elem != NUL) {
			parent = *elem;
			if (key < (*elem)->keyval.Key()) elem = &((*elem)->left);
			else if (key > (*elem)->keyval.Key()) elem = &((*elem)->right);
			else ret NO;
		}
		*elem = &tx->ElemAlloc(keyval, parent);
		tx->TravelUp(*elem);
		ret YES;
	}
	dfa BO Del(cx T1& key) {
		DictAvlElem* elem = tx->Srch(key);
		ifu (elem == NUL) ret NO;
		tx->Del(elem);
		ret YES;
	}
	dfa T2* Get(cx T1& key) {
		DictAvlElem* elem = tx->Srch(key);
		ifu (elem == NUL) ret NUL;
		ret &(const_cast<T2&>(elem->keyval.Val()));
	}
	dfa SI Len() {
		ret tx->_Len(m_root);
	}
	dfa SI Height() {
		ret tx->_Height(m_root);
	}
	dfa NT Free() {
		tx->_Free(m_root);
		m_root = NUL;
	}
	dfa DictAvl() {
		m_root = NUL;
	}
	dfa BO operator += (cx Keyval<T1, T2>& keyval) {
		ret tx->Add(keyval);
	}
	dfa BO operator -= (cx T1& key) {
		ret tx->Del(key);
	}
	dfa T2* operator [] (cx T1& key) {
		ret tx->Get(key);
	}
};
