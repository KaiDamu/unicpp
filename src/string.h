#pragma once

constexpr SI STR_INIT_ALLOC_CNT = 8;
constexpr F8 STR_ADD_ALLOC_SCALE = 1.5;

dfa SI CsstrLen(cx CS* str) {
	ret __builtin_strlen(str);
}
dfa NT CsstrCpy(CS* dst, cx CS* src) {
	#ifdef PROG_COMPILER_GCC
		__builtin_strcpy(dst, src);
	#else
		strcpy(dst, src);
	#endif
}
dfa NT CsstrAdd(CS* dst, cx CS* src) {
	#ifdef PROG_COMPILER_GCC
		__builtin_strcat(dst, src);
	#else
		strcat(dst, src);
	#endif
}
dfa SA CsstrCmp(cx CS* strA, cx CS* strB) {
	#ifdef PROG_COMPILER_GCC
		ret SA(__builtin_strcmp(strA, strB));
	#else
		ret SA(strcmp(strA, strB));
	#endif
}
dfa SI CsstrEnclose(CS* dst, cx CS* src, cx CS* left, cx CS* right) {
	cx SI leftLen = CsstrLen(left);
	MemCpy(dst, left, leftLen * siz(CS));
	dst += leftLen;
	cx SI srcLen = CsstrLen(src);
	MemCpy(dst, src, srcLen * siz(CS));
	dst += srcLen;
	cx SI rightLen = CsstrLen(right);
	MemCpy(dst, right, rightLen * siz(CS));
	dst += rightLen;
	*dst = '\0';
	ret leftLen + srcLen + rightLen;
}
dfa SI CsstrReplace(CS* dst, cx CS* str, SI i, SI len) {
	i += len;
	SI dstLen = CsstrLen(dst);
	cx SI strLen = CsstrLen(str) - len;
	CS* dstP = dst + dstLen;
	if (dstP < dst + i) {
		dstLen += dst + i - dstP;
		*(dst + dstLen + strLen) = '\0';
	}
	while (dstP >= dst + i) {
		*(dstP + strLen) = *dstP;
		--dstP;
	}
	MemCpy(dst + i - len, str, (strLen + len) * siz(CS));
	ret dstLen + strLen;
}
dfa SI CsstrInsert(CS* dst, cx CS* str, SI i) {
	ret CsstrReplace(dst, str, i, 0);
}
dfa cx CS* CsstrFind(cx CS* main, CS c) {
	cx CS* p = main;
	while (*p != '\0') {
		if (*p == c) ret p;
		++p;
	}
	ret NUL;
}
dfa SI CsstrFindI(cx CS* main, CS c) {
	cx CS*cx p = CsstrFind(main, c);
	ret (p == NUL) ? -1 : p - main;
}
dfa cx CS* CsstrFind(cx CS* main, cx CS* sub) {
	#ifdef PROG_COMPILER_GCC
		ret __builtin_strstr(main, sub);
	#else
		ret strstr(main, sub);
	#endif
}
dfa SI CsstrFindI(cx CS* main, cx CS* sub) {
	cx CS*cx p = CsstrFind(main, sub);
	ret (p == NUL) ? -1 : p - main;
}
dfa NT CsstrSubChar(CS* dst, cx CS* charList) {
	cx SI charListLen = CsstrLen(charList);
	cx CS* src = dst;
	while (*src != '\0') {
		cx CS* charListP = charList;
		while (*charListP != '\0') {
			if (*src == *charListP) {
				++src;
				break;
			}
			++charListP;
		}
		if (charListP == charList + charListLen) {
			*dst = *src;
			++dst;
			++src;
		}
	}
	*dst = '\0';
}
dfa NT CsstrTrimWspace(CS* dst) {
	cx CS* src = dst;
	while (IsWspace(*src)) ++src;
	CS* dstP = dst;
	while (*src != '\0') {
		*dstP = *src;
		++dstP;
		++src;
	}
	while (dstP > dst && IsWspace(*(dstP - 1))) --dstP;
	*dstP = '\0';
}

dfa SI ChstrLen(cx CH* str) {
	ret wcslen(str);
}
dfa NT ChstrCpy(CH* dst, cx CH* src) {
	wcscpy(dst, src);
}
dfa NT ChstrAdd(CH* dst, cx CH* src) {
	wcscat(dst, src);
}
dfa SA ChstrCmp(cx CH* strA, cx CH* strB) {
	ret SA(wcscmp(strA, strB));
}
dfa SI ChstrEnclose(CH* dst, cx CH* src, cx CH* left, cx CH* right) {
	cx SI leftLen = ChstrLen(left);
	MemCpy(dst, left, leftLen * siz(CH));
	dst += leftLen;
	cx SI srcLen = ChstrLen(src);
	MemCpy(dst, src, srcLen * siz(CH));
	dst += srcLen;
	cx SI rightLen = ChstrLen(right);
	MemCpy(dst, right, rightLen * siz(CH));
	dst += rightLen;
	*dst = '\0';
	ret leftLen + srcLen + rightLen;
}
dfa SI ChstrReplace(CH* dst, cx CH* str, SI i, SI len) {
	i += len;
	SI dstLen = ChstrLen(dst);
	cx SI strLen = ChstrLen(str) - len;
	CH* dstP = dst + dstLen;
	if (dstP < dst + i) {
		dstLen += dst + i - dstP;
		*(dst + dstLen + strLen) = '\0';
	}
	while (dstP >= dst + i) {
		*(dstP + strLen) = *dstP;
		--dstP;
	}
	MemCpy(dst + i - len, str, (strLen + len) * siz(CH));
	ret dstLen + strLen;
}
dfa SI ChstrInsert(CH* dst, cx CH* str, SI i) {
	ret ChstrReplace(dst, str, i, 0);
}
dfa cx CH* ChstrFind(cx CH* main, CH c) {
	cx CH* p = main;
	while (*p != '\0') {
		if (*p == c) ret p;
		++p;
	}
	ret NUL;
}
dfa SI ChstrFindI(cx CH* main, CH c) {
	cx CH*cx p = ChstrFind(main, c);
	ret (p == NUL) ? -1 : p - main;
}
dfa cx CH* ChstrFind(cx CH* main, cx CH* sub) {
	ret wcsstr(main, sub);
}
dfa SI ChstrFindI(cx CH* main, cx CH* sub) {
	cx CH*cx p = ChstrFind(main, sub);
	ret (p == NUL) ? -1 : p - main;
}
dfa NT ChstrSubChar(CH* dst, cx CH* charList) {
	cx SI charListLen = ChstrLen(charList);
	cx CH* src = dst;
	while (*src != '\0') {
		cx CH* charListP = charList;
		while (*charListP != '\0') {
			if (*src == *charListP) {
				++src;
				break;
			}
			++charListP;
		}
		if (charListP == charList + charListLen) {
			*dst = *src;
			++dst;
			++src;
		}
	}
	*dst = '\0';
}
dfa NT ChstrTrimWspace(CH* dst) {
	cx CH* src = dst;
	while (IsWspace(*src)) ++src;
	CH* dstP = dst;
	while (*src != '\0') {
		*dstP = *src;
		++dstP;
		++src;
	}
	while (dstP > dst && IsWspace(*(dstP - 1))) --dstP;
	*dstP = '\0';
}

class Str {
private:
	Arr<CH> m_str;
private:
	dfa NT Init() {
		m_str.Alloc(STR_INIT_ALLOC_CNT);
		m_str.Set('\0');
	}
public:
	dfa SI Len() const {
		ret m_str.Pos() - 1;
	}
	dfa cx CH* Val() const {
		ret m_str.Ptr();
	}
public:
	dfa NT Set(cx CH* str, SI strLen) {
		m_str.Req(strLen + 1, strLen + 1, 0);
		m_str.Set(str, strLen);
		m_str.Add('\0');
	}
	dfa NT Add(cx CH* str, SI strLen) {
		cx SI fullLen = tx->Len() + strLen + 1;
		m_str.Req(fullLen, SI(F8(fullLen) * STR_ADD_ALLOC_SCALE), m_str.Pos());
		m_str.CurMove(-1);
		m_str.Add(str, strLen);
		m_str.Add('\0');
	}
public:
	dfa cx CH* operator = (cx CH* str) {
		tx->Set(str, ChstrLen(str));
		ret tx->Val();
	}
	dfa cx CH* operator += (cx CH* str) {
		tx->Add(str, ChstrLen(str));
		ret tx->Val();
	}
	dfa BO operator == (cx CH* str) const { ret (ChstrCmp(tx->Val(), str) == 0); }
	dfa BO operator != (cx CH* str) const { ret (ChstrCmp(tx->Val(), str) != 0); }
	dfa BO operator <  (cx CH* str) const { ret (ChstrCmp(tx->Val(), str) <  0); }
	dfa BO operator >  (cx CH* str) const { ret (ChstrCmp(tx->Val(), str) >  0); }
	dfa BO operator <= (cx CH* str) const { ret (ChstrCmp(tx->Val(), str) <= 0); }
	dfa BO operator >= (cx CH* str) const { ret (ChstrCmp(tx->Val(), str) >= 0); }
public:
	dfa cx CH* operator = (cx Str& str) {
		tx->Set(str.Val(), str.Len());
		ret tx->Val();
	}
	dfa cx CH* operator += (cx Str& str) {
		tx->Add(str.Val(), str.Len());
		ret tx->Val();
	}
	dfa BO operator == (cx Str& str) const { ret ((tx->Len() == str.Len()) && (ChstrCmp(tx->Val(), str.Val()) == 0)); }
	dfa BO operator != (cx Str& str) const { ret ((tx->Len() != str.Len()) || (ChstrCmp(tx->Val(), str.Val()) != 0)); }
	dfa BO operator <  (cx Str& str) const { ret (ChstrCmp(tx->Val(), str.Val()) <  0); }
	dfa BO operator >  (cx Str& str) const { ret (ChstrCmp(tx->Val(), str.Val()) >  0); }
	dfa BO operator <= (cx Str& str) const { ret (ChstrCmp(tx->Val(), str.Val()) <= 0); }
	dfa BO operator >= (cx Str& str) const { ret (ChstrCmp(tx->Val(), str.Val()) >= 0); }
public:
	dfa Str() {
		tx->Init();
	}
	dfa Str(cx CH* str) {
		tx->Init();
		tx->operator=(str);
	}
	dfa Str(cx Str& str) {
		tx->Init();
		tx->operator=(str);
	}
};

class SStr {
private:
	Arr<CS> m_str;
private:
	dfa NT Init() {
		m_str.Alloc(STR_INIT_ALLOC_CNT);
		m_str.Set('\0');
	}
public:
	dfa SI Len() const {
		ret m_str.Pos() - 1;
	}
	dfa cx CS* Val() const {
		ret m_str.Ptr();
	}
public:
	dfa NT Set(cx CS* str, SI strLen) {
		m_str.Req(strLen + 1, strLen + 1, 0);
		m_str.Set(str, strLen);
		m_str.Add('\0');
	}
	dfa NT Add(cx CS* str, SI strLen) {
		cx SI fullLen = tx->Len() + strLen + 1;
		m_str.Req(fullLen, SI(F8(fullLen) * STR_ADD_ALLOC_SCALE), m_str.Pos());
		m_str.CurMove(-1);
		m_str.Add(str, strLen);
		m_str.Add('\0');
	}
public:
	dfa cx CS* operator = (cx CS* str) {
		tx->Set(str, CsstrLen(str));
		ret tx->Val();
	}
	dfa cx CS* operator += (cx CS* str) {
		tx->Add(str, CsstrLen(str));
		ret tx->Val();
	}
	dfa BO operator == (cx CS* str) const { ret (CsstrCmp(tx->Val(), str) == 0); }
	dfa BO operator != (cx CS* str) const { ret (CsstrCmp(tx->Val(), str) != 0); }
	dfa BO operator <  (cx CS* str) const { ret (CsstrCmp(tx->Val(), str) <  0); }
	dfa BO operator >  (cx CS* str) const { ret (CsstrCmp(tx->Val(), str) >  0); }
	dfa BO operator <= (cx CS* str) const { ret (CsstrCmp(tx->Val(), str) <= 0); }
	dfa BO operator >= (cx CS* str) const { ret (CsstrCmp(tx->Val(), str) >= 0); }
public:
	dfa cx CS* operator = (cx SStr& str) {
		tx->Set(str.Val(), str.Len());
		ret tx->Val();
	}
	dfa cx CS* operator += (cx SStr& str) {
		tx->Add(str.Val(), str.Len());
		ret tx->Val();
	}
	dfa BO operator == (cx SStr& str) const { ret ((tx->Len() == str.Len()) && (CsstrCmp(tx->Val(), str.Val()) == 0)); }
	dfa BO operator != (cx SStr& str) const { ret ((tx->Len() != str.Len()) || (CsstrCmp(tx->Val(), str.Val()) != 0)); }
	dfa BO operator <  (cx SStr& str) const { ret (CsstrCmp(tx->Val(), str.Val()) <  0); }
	dfa BO operator >  (cx SStr& str) const { ret (CsstrCmp(tx->Val(), str.Val()) >  0); }
	dfa BO operator <= (cx SStr& str) const { ret (CsstrCmp(tx->Val(), str.Val()) <= 0); }
	dfa BO operator >= (cx SStr& str) const { ret (CsstrCmp(tx->Val(), str.Val()) >= 0); }
public:
	dfa SStr() {
		tx->Init();
	}
	dfa SStr(cx CS* str) {
		tx->Init();
		tx->operator=(str);
	}
	dfa SStr(cx SStr& str) {
		tx->Init();
		tx->operator=(str);
	}
};
