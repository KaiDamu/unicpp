#pragma once

class RandCtx {
private:
	U4 m_i;
	U4 m_val[624];
private:
	dfa NT Init(U4 seed) {
		m_val[0] = seed;
		for (m_i = 1; m_i < 624; ++m_i) {
			m_val[m_i] = m_val[m_i - 1] * 6069;
		}
	}
	dfa NT Roll(U4 i11, U4 i12, U4 i21, U4 i22) {
		cx U4 val = (m_val[i11] & 0x80000000) | (m_val[i12] & 0x7FFFFFFF);
		m_val[i21] = m_val[i22] ^ (val >> 1) ^ ((val & 0x01) * 0x9908B0DF);
	}
	dfa NT Gen() {
		U4 i = 0;
		while (i < 227) {
			tx->Roll(i, i + 1, i, i + 397);
			++i;
		}
		while (i < 623) {
			tx->Roll(i, i + 1, i, i - 227);
			++i;
		}
		tx->Roll(623, 0, 623, 396);
		m_i = 0;
	}
	dfa U4 Next() {
		ifu (m_i == 624) {
			tx->Gen();
		}
		U4 r = m_val[m_i++];
		r ^= r >> 11;
		r ^= (r << 7) & 0x9D2C5680;
		r ^= (r << 15) & 0xEFC60000;
		r ^= r >> 18;
		ret r;
	}
public:
	dfa F8 RandNormal() {
		ret U4ToF8(tx->Next()) * 2.32830643654e-10;
	}
	dfa U4 RandU4() {
		ret tx->Next();
	}
	dfa U4 RandU4(U4 min, U4 max) {
		ret F8ToU4(U4ToF8(min + (max - min + 1)) * tx->RandNormal());
	}
	dfa S4 RandS4(S4 min, S4 max) {
		ret F8ToS4(S4ToF8(min + (max - min + 1)) * tx->RandNormal());
	}
	dfa F4 RandF4(F4 min, F4 max) {
		ret F8ToF4(F4ToF8(min + (max - min + 1)) * tx->RandNormal());
	}
	dfa F8 RandF8(F8 min, F8 max) {
		ret (min + (max - min + 1))* tx->RandNormal();
	}
public:
	dfa RandCtx() {
		tx->Init(4357);
	}
	dfa RandCtx(U4 seed) {
		tx->Init(seed);
	}
};

RandCtx g_randCtx(U4(TimeUnixGetSecU() + CpuTsc()));

dfa F8 RandNormal() {
	ret g_randCtx.RandNormal();
}
dfa U4 RandU4() {
	ret g_randCtx.RandU4();
}
dfa U4 RandU4(U4 min, U4 max) {
	ret g_randCtx.RandU4(min, max);
}
dfa S4 RandS4(S4 min, S4 max) {
	ret g_randCtx.RandS4(min, max);
}
dfa F4 RandF4(F4 min, F4 max) {
	ret g_randCtx.RandF4(min, max);
}
dfa F8 RandF8(F8 min, F8 max) {
	ret g_randCtx.RandF8(min, max);
}

tpl1 dfa NT RandMix(T1* buf, SI cnt) {
	ite (i, i < cnt) Swap<T1>(buf[i], buf[SI(RandU4(0, U4(cnt - 1)))]);
}
