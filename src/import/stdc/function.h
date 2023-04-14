#pragma once

#define _importfnstdc(retType) __declspec(dllimport) retType __cdecl

extern "C" {
	_importfnstdc(double)sqrt(double);
}
