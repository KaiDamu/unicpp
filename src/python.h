#pragma once

dfa ER PythonExePathGet(CH* path) {
	ife (PathEnvRelToAbs(path, L"python.exe")) retep;
	rets;
}
dfa ER PythonStart(cx CH* file, cx CH* args, BO wait) {
	CH path[PATH_LEN_MAX];
	ife (PythonExePathGet(path)) retep;
	Str _args;
	_args = L"\"";
	_args += file;
	_args += L"\"";
	if (args != NUL) {
		_args += L" ";
		_args += args;
	}
	Proc proc;
	ife (proc.Start(path, _args.Get(), NUL)) retep;
	if (wait) {
		ife (proc.Wait()) retep;
	} else {
		proc.__Drop();
	}
	rets;
}
