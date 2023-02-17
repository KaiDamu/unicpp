#pragma once

dfa S4 F4ToS4Fast(F4 val) {
	S4 r;
	#ifdef PROG_COMPILER_GCC
		__asm__("fistpl %0" : "=m"(r) : "t"(val) : "st");
	#else
		r = S4(val);
	#endif
	ret r;
}

dfa F8 U8ToF8(U8 val) {
	ret F8(val);
}
dfa F8 S8ToF8(S8 val) {
	ret F8(val);
}

dfa F8 U4ToF8(U4 val) {
	ret F8(val);
}
dfa F8 S4ToF8(S4 val) {
	ret F8(val);
}
dfa F8 F4ToF8(F4 val) {
	ret F8(val);
}

dfa U4 F8ToU4(F8 val) {
	ret U4(val);
}
dfa S4 F8ToS4(F8 val) {
	ret S4(val);
}
dfa F4 F8ToF4(F8 val) {
	ret F4(val);
}

dfa F4 U4ToF4(U4 val) {
	ret F4(val);
}
dfa F4 S4ToF4(S4 val) {
	ret F4(val);
}

tpl1 SI U4ToStr(T1* dst, U4 src) {
	cx SI len = LenU4(src);
	*(dst += len) = '\0';
	switch (len) {
	case 10: *(--dst) = T1(src % 10 + '0'); src /= 10; falltru;
	case  9: *(--dst) = T1(src % 10 + '0'); src /= 10; falltru;
	case  8: *(--dst) = T1(src % 10 + '0'); src /= 10; falltru;
	case  7: *(--dst) = T1(src % 10 + '0'); src /= 10; falltru;
	case  6: *(--dst) = T1(src % 10 + '0'); src /= 10; falltru;
	case  5: *(--dst) = T1(src % 10 + '0'); src /= 10; falltru;
	case  4: *(--dst) = T1(src % 10 + '0'); src /= 10; falltru;
	case  3: *(--dst) = T1(src % 10 + '0'); src /= 10; falltru;
	case  2: *(--dst) = T1(src % 10 + '0'); src /= 10; falltru;
	default: *(--dst) = T1(src % 10 + '0');
	}
	ret len;
}
