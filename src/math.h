#pragma once

tpl1 dfa T1 Min(T1 a, T1 b) {
	ret ((a < b) ? a : b);
}
tpl1 dfa T1 Max(T1 a, T1 b) {
	ret ((a > b) ? a : b);
}
tpl1 dfa T1 Clamp(T1 val, T1 min, T1 max) {
	ret Min<T1>(Max<T1>(val, min), max);
}
tpl1 dfa T1 Abs(T1 val) {
	ret ((val < 0) ? -val : val);
}
tpl1 dfa T1 Sign(T1 val) {
	ret ((val < 0) ? -1 : 1);
}
tpl1 dfa T1 Square(T1 val) {
	ret val * val;
}
tpl1 dfa T1 AlignBit(T1 val, T1 size) {
	ret (val + (size - 1)) & ~(size - 1);
}
tpl1 dfa NT Swap(T1& a, T1& b) {
	cx T1 tmp = a;
	a = b;
	b = tmp;
}
tpl2 dfa T1 Lerp(T1 a, T1 b, T2 t) {
	ret a + T1(T2(b - a) * t);
}

dfa S4 RoundF4ToS4(F4 val) {
	ret S4((val < 0.0f) ? (val - 0.5f) : (val + 0.5f));
}
dfa S8 RoundF8ToS8(F8 val) {
	ret S8((val < 0.0) ? (val - 0.5) : (val + 0.5));
}

tpl1 dfa SI VarintEncode(U1* out, T1 in) {
	cx U1*cx outBase = out;
	while (in > 0x7F) {
		*out++ = U1(in & 0x7F) | U1(0x80);
		in >>= 7;
	}
	*out++ = U1(in & 0x7F);
	ret SI(out - outBase);
}
tpl1 dfa SI VarintDecode(T1& out, cx U1* in) {
	out = 0;
	cx U1*cx inBase = in;
	do {
		out |= ((*in) & 0x7F) << (0x07 * U1(in - inBase));
		if (!(*in & 0x80)) {
			++in;
			break;
		}
		++in;
	} while (YES);
	ret SI(in - inBase);
}

dfa U1 ByteObfuscate(U1 val, U1 i) {
	ret U1((val ^ 0x55) + (i ^ 0xAA));
}
dfa U1 ByteUnobfuscate(U1 val, U1 i) {
	ret U1(val - (i ^ 0xAA)) ^ 0x55;
}
