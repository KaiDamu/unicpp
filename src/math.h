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
	SI outSize = 0;
	while (in > 0x7F) {
		out[outSize++] = U1(in & 0x7F) | 0x80;
		in >>= 7;
	}
	out[outSize++] = U1(in) & 0x7F;
	ret outSize;
}
tpl1 dfa T1 VarintDecode(cx U1* in, SI inSize, SI& readSize) {
	T1 out = 0;
	SI i = 0;
	while (i != inSize) {
		out |= (in[i] & 0x7F) << (0x07 * i);
		if (!(in[i] & 0x80)) {
			++i;
			break;
		}
		++i;
	}
	readSize = i;
	ret out;
}
tpl1 dfa SI VarintDecode(T1& out, cx U1* in) {
	out = 0;
	SI i = 0;
	do {
		out |= (in[i] & 0x7F) << (0x07 * i);
		if (!(in[i] & 0x80)) {
			++i;
			break;
		}
		++i;
	} while (YES);
	ret i;
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
