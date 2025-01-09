#pragma once

dfa ER PythonExePathGet(CH* path)
{
    ifu (PathEnvRelToAbs(path, L"python.exe") == 0)
        rete(ErrVal::ENV);
    rets;
}
dfa ER PythonStart(cx CH* file, cx CH* args, BO wait)
{
    CH path[PATH_LEN_MAX];
    ife (PythonExePathGet(path))
        retep;
    std::wstring _args = L"\"";
    _args += file;
    _args += L"\"";
    if (args != NUL)
    {
        _args += L" ";
        _args += args;
    }
    Proc proc;
    ife (proc.Start(path, _args.c_str(), NUL))
        retep;
    if (wait)
    {
        ife (proc.Wait())
            retep;
    }
    else
    {
        proc.__Drop();
    }
    rets;
}
