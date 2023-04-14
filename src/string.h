#pragma once

constexpr SI STR_EX_LEN = 1;

dfa SI StrLen(cx CS* str) {
	ret __builtin_strlen(str);
}
dfa SI StrLen(cx CH* str) {
	ret wcslen(str);
}
dfa NT StrSet(CS* dst, cx CS* src) {
	#ifdef PROG_COMPILER_GCC
		__builtin_strcpy(dst, src);
	#else
		strcpy(dst, src);
	#endif
}
dfa NT StrSet(CH* dst, cx CH* src) {
	wcscpy(dst, src);
}
dfa NT StrAdd(CS* dst, cx CS* src) {
	#ifdef PROG_COMPILER_GCC
		__builtin_strcat(dst, src);
	#else
		strcat(dst, src);
	#endif
}
dfa NT StrAdd(CH* dst, cx CH* src) {
	wcscat(dst, src);
}
dfa SA StrCmp(cx CS* strA, cx CS* strB) {
	#ifdef PROG_COMPILER_GCC
		ret SA(__builtin_strcmp(strA, strB));
	#else
		ret SA(strcmp(strA, strB));
	#endif
}
dfa SA StrCmp(cx CH* strA, cx CH* strB) {
	ret SA(wcscmp(strA, strB));
}
dfa SA StrCmp(cx CS* strA, cx CS* strB, SI len) {
	#ifdef PROG_COMPILER_GCC
		ret SA(__builtin_strncmp(strA, strB, len));
	#else
		ret SA(strncmp(strA, strB, len));
	#endif
}
dfa SA StrCmp(cx CH* strA, cx CH* strB, SI len) {
	ret SA(wcsncmp(strA, strB, len));
}
dfa cx CS* StrFind(cx CS* main, cx CS* sub) {
	#ifdef PROG_COMPILER_GCC
		ret __builtin_strstr(main, sub);
	#else
		ret strstr(main, sub);
	#endif
}
dfa cx CH* StrFind(cx CH* main, cx CH* sub) {
	ret wcsstr(main, sub);
}

dfa SI CsstrToChstr(CH* dst, cx CS* src) {
	cx CH* cx dstBase = dst;
	while (*src != '\0') *dst++ = *src++;
	*dst = '\0';
	ret SI(dst - dstBase);
}
dfa SI ChstrToCsstr(CS* dst, cx CH* src) {
	cx CS* cx dstBase = dst;
	while (*src != '\0') {
		if (*src > CH(CS_VAL_MAX)) *dst = CH_NA;
		else *dst = CS(*src);
		++dst;
		++src;
	}
	*dst = '\0';
	ret SI(dst - dstBase);
}

tpl1 dfa SI StrEnclose(T1* dst, cx T1* src, cx T1* left, cx T1* right) {
	cx SI leftLen = StrLen(left);
	MemSet(dst, left, leftLen * siz(T1));
	dst += leftLen;
	cx SI srcLen = StrLen(src);
	MemSet(dst, src, srcLen * siz(T1));
	dst += srcLen;
	cx SI rightLen = StrLen(right);
	MemSet(dst, right, rightLen * siz(T1));
	dst += rightLen;
	*dst = '\0';
	ret leftLen + srcLen + rightLen;
}
tpl1 dfa SI StrReplace(T1* dst, cx T1* str, SI i, SI len) {
	i += len;
	SI dstLen = StrLen(dst);
	cx SI strLen = StrLen(str) - len;
	T1* dstP = dst + dstLen;
	if (dstP < dst + i) {
		dstLen += dst + i - dstP;
		*(dst + dstLen + strLen) = '\0';
	}
	while (dstP >= dst + i) {
		*(dstP + strLen) = *dstP;
		--dstP;
	}
	MemSet(dst + i - len, str, (strLen + len) * siz(T1));
	ret dstLen + strLen;
}
tpl1 dfa SI StrInsert(T1* dst, cx T1* str, SI i) {
	ret StrReplace(dst, str, i, 0);
}
tpl1 dfa SI StrSubChar(T1* dst, cx T1* charList) {
	cx T1*cx dstBase = dst;
	cx SI charListLen = StrLen(charList);
	cx T1* src = dst;
	while (*src != '\0') {
		cx T1* charListP = charList;
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
	ret dst - dstBase;
}
tpl1 dfa SI StrTrimWspace(T1* dst) {
	cx T1* src = dst;
	while (IsWspace(*src)) ++src;
	T1* dstP = dst;
	while (*src != '\0') {
		*dstP = *src;
		++dstP;
		++src;
	}
	while (dstP > dst && IsWspace(*(dstP - 1))) --dstP;
	*dstP = '\0';
	ret dstP - dst;
}
tpl1 dfa cx T1* StrFind(cx T1* main, cx T1& c) {
	cx T1* p = main;
	while (*p != '\0') {
		if (*p == c) ret p;
		++p;
	}
	ret NUL;
}
tpl1 dfa cx T1* StrFindLast(cx T1* main, cx T1& c) {
	cx T1* p = main + StrLen(main) - 1;
	while (p >= main) {
		if (*p == c) ret p;
		--p;
	}
	ret NUL;
}
tpl1 dfa SI StrFindI(cx T1* main, cx T1& c) {
	cx T1*cx p = StrFind(main, c);
	ret (p == NUL) ? -1 : p - main;
}
tpl1 dfa SI StrFindI(cx T1* main, cx T1* sub) {
	cx T1*cx p = StrFind(main, sub);
	ret (p == NUL) ? -1 : p - main;
}
tpl1 dfa cx T1* StrArgSkip(cx T1* str) {
	cx T1* p = str;
	if (*p == '\"') {
		++p;
		while (*p != '\0' && *p != '\"') ++p;
		if (*p == '\"') ++p;
	} else {
		while (*p != '\0' && *p != ' ') ++p;
	}
	while (*p == ' ') ++p;
	ret p;
}
tpl1 dfa BO StrIsFirst(cx T1* main, cx T1* sub) {
	cx T1* p1 = main;
	cx T1* p2 = sub;
	while (*p1 != '\0' && *p2 != '\0' && *p1 == *p2) {
		++p1;
		++p2;
	}
	ret *p2 == '\0';
}

tpl1 dfa SI StrSetLenGet(T1* dst, cx T1* src) {
	T1* p = dst;
	while (*src != '\0') *p++ = *src++;
	*p = '\0';
	ret p - dst;
}

constexpr SI STR_INIT_ALLOC_CNT = 8;
constexpr F8 STR_ADD_ALLOC_SCALE = 1.5;

tpl1 class StrBase {
private:
	Arr<T1> m_str;
private:
	dfa NT Init() {
		m_str.New(STR_INIT_ALLOC_CNT);
		m_str.Set('\0');
	}
public:
	dfa SI Len() const {
		ret m_str.Pos() - 1;
	}
	dfa cx T1* Get() const {
		ret m_str.Ptr();
	}
public:
	dfa NT Set(cx T1* str, SI strLen) {
		m_str.Req(strLen + 1, strLen + 1, 0);
		m_str.Set(str, strLen);
		m_str.Write('\0');
	}
	dfa NT Add(cx T1* str, SI strLen) {
		cx SI fullLen = tx->Len() + strLen + 1;
		m_str.Req(fullLen, SI(F8(fullLen) * STR_ADD_ALLOC_SCALE), m_str.Pos());
		m_str.CurMove(-1);
		m_str.Write(str, strLen);
		m_str.Write('\0');
	}
public:
	dfa cx T1* operator = (cx T1* str) {
		tx->Set(str, StrLen(str));
		ret tx->Get();
	}
	dfa cx T1* operator += (cx T1* str) {
		tx->Add(str, StrLen(str));
		ret tx->Get();
	}
	dfa BO operator == (cx T1* str) const { ret (StrCmp(tx->Get(), str) == 0); }
	dfa BO operator != (cx T1* str) const { ret (StrCmp(tx->Get(), str) != 0); }
	dfa BO operator <  (cx T1* str) const { ret (StrCmp(tx->Get(), str) <  0); }
	dfa BO operator >  (cx T1* str) const { ret (StrCmp(tx->Get(), str) >  0); }
	dfa BO operator <= (cx T1* str) const { ret (StrCmp(tx->Get(), str) <= 0); }
	dfa BO operator >= (cx T1* str) const { ret (StrCmp(tx->Get(), str) >= 0); }
public:
	dfa cx T1* operator = (cx StrBase& str) {
		tx->Set(str.Get(), str.Len());
		ret tx->Get();
	}
	dfa cx T1* operator += (cx StrBase& str) {
		tx->Add(str.Get(), str.Len());
		ret tx->Get();
	}
	dfa BO operator == (cx StrBase& str) const { ret ((tx->Len() == str.Len()) && (StrCmp(tx->Get(), str.Get()) == 0)); }
	dfa BO operator != (cx StrBase& str) const { ret ((tx->Len() != str.Len()) || (StrCmp(tx->Get(), str.Get()) != 0)); }
	dfa BO operator <  (cx StrBase& str) const { ret (StrCmp(tx->Get(), str.Get()) <  0); }
	dfa BO operator >  (cx StrBase& str) const { ret (StrCmp(tx->Get(), str.Get()) >  0); }
	dfa BO operator <= (cx StrBase& str) const { ret (StrCmp(tx->Get(), str.Get()) <= 0); }
	dfa BO operator >= (cx StrBase& str) const { ret (StrCmp(tx->Get(), str.Get()) >= 0); }
public:
	dfa StrBase() {
		tx->Init();
	}
	dfa StrBase(cx T1* str) {
		tx->Init();
		tx->operator=(str);
	}
	dfa StrBase(cx StrBase& str) {
		tx->Init();
		tx->operator=(str);
	}
};

using Str = StrBase<CH>;
using SStr = StrBase<CS>;

dfa Str ToStr(cx SStr& str) {
	Str r;
	Arr<CH> tmp(str.Len() + 1);
	CsstrToChstr(tmp.Ptr(), str.Get());
	r.Set(tmp.Ptr(), str.Len());
	ret r;
}
dfa Str ToStr(U4 val) {
	CH buf[16];
	U4ToStr(buf, val);
	ret Str(buf);
}
