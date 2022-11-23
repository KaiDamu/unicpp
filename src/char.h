#pragma once

enum : CS {
	CH_NUL = 0,
	CH_SOH = 1,
	CH_STX = 2,
	CH_ETX = 3,
	CH_EOT = 4,
	CH_ENQ = 5,
	CH_ACK = 6,
	CH_BEL = 7,
	CH_BS = 8,
	CH_HT = 9,
	CH_LF = 10,
	CH_VT = 11,
	CH_FF = 12,
	CH_CR = 13,
	CH_SO = 14,
	CH_SI = 15,
	CH_DLE = 16,
	CH_DC1 = 17,
	CH_DC2 = 18,
	CH_DC3 = 19,
	CH_DC4 = 20,
	CH_NAK = 21,
	CH_SYN = 22,
	CH_ETB = 23,
	CH_CAN = 24,
	CH_EM = 25,
	CH_SUB = 26,
	CH_ESC = 27,
	CH_FS = 28,
	CH_GS = 29,
	CH_RS = 30,
	CH_US = 31,
	CH_DEL = 127
};

constexpr CS CH_UNKNOWN = '?';

dfa BO IsBetween(CS c, CS min, CS max) {
	ret c >= min && c <= max;
}
dfa BO IsLowcase(CS c) {
	ret IsBetween(c, 'a', 'z');
}
dfa BO IsUpcase(CS c) {
	ret IsBetween(c, 'A', 'Z');
}
dfa BO IsNumBase2(CS c) {
	ret IsBetween(c, '0', '1');
}
dfa BO IsNumBase8(CS c) {
	ret IsBetween(c, '0', '7');
}
dfa BO IsNumBase10(CS c) {
	ret IsBetween(c, '0', '9');
}
dfa BO IsNumBase16(CS c) {
	ret IsNumBase10(c) || IsBetween(c, 'A', 'F') || IsBetween(c, 'a', 'f');
}
dfa BO IsSpace(CS c) {
	ret c == ' ';
}
dfa BO IsTab(CS c) {
	ret c == CH_HT;
}
dfa BO IsEol(CS c) {
	ret c == CH_CR || c == CH_LF;
}
dfa BO IsCntrl(CS c) {
	ret IsBetween(c, 0, 31) || c == 127;
}
dfa BO IsAscii(CS c) {
	ret IsBetween(c, 0, 127);
}
