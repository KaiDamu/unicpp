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

class Str {
private:
	Arr<CH> m_str;
private:
	dfa NT Set(cx CH* str, SI datLen) {
		m_str.Req(datLen, datLen, 0);
		m_str.Set(str, datLen);
	}
	dfa NT Add(cx CH* str, SI datLen) {
		cx SI fullLen = tx->Len() + datLen;
		m_str.CurMove(-1);
		m_str.Req(fullLen, SI(F8(fullLen) * STR_ADD_ALLOC_SCALE), m_str.Pos());
		m_str.Add(str, datLen);
	}
public:
	dfa SI Len() {
		ret m_str.Pos() - 1;
	}
	dfa cx CH* Val() {
		ret m_str.Ptr();
	}
public:
	dfa NT operator = (cx CH* str) {
		cx SI datLen = ChstrLen(str) + 1;
		tx->Set(str, datLen);
	}
	dfa NT operator += (cx CH* str) {
		cx SI datLen = ChstrLen(str) + 1;
		tx->Add(str, datLen);
	}
public:
	dfa Str() {
		m_str.Alloc(STR_INIT_ALLOC_CNT);
		m_str.Set('\0');
	}
};
