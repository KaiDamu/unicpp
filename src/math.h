#pragma once

tpl1 dfa T1 Min(T1 a, T1 b) {
	ret ((a < b) ? a : b);
}
tpl1 dfa T1 Max(T1 a, T1 b) {
	ret ((a > b) ? a : b);
}

tpl1 dfa NT Swap(T1& a, T1& b) {
	cx T1 tmp = a;
	a = b;
	b = tmp;
}

tpl1 dfa SI VarintEncode(U1* out, T1 in) {
	U1*cx outBase = out;
	while (in > 0x7F) {
		*out++ = U1(in & 0x7F) | 0x80;
		in >>= 7;
	}
	*out++ = U1(in) & 0x7F;
	ret SI(out - outBase);
}
tpl1 dfa SI VarintDecode(T1& out, cx U1* in) {
	out = 0;
	cx U1*cx inBase = in;
	do {
		out |= ((*in) & 0x7F) << (0x07 * (in - inBase));
		if (!(*in & 0x80)) {
			++in;
			break;
		}
		++in;
	} while (YES);
	ret SI(in - inBase);
}

dfa UA BitAlign(UA val, SI size) {
	ret (val + (size - 1)) & ~(size - 1);
}

dfa U1 ObfuscateByte(U1 val, U1 i) {
	ret U1((val ^ 0x55) + (i ^ 0xAA));
}
dfa U1 UnobfuscateByte(U1 val, U1 i) {
	ret U1(val - (i ^ 0xAA)) ^ 0x55;
}
