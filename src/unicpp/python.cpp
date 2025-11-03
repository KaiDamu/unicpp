#include "python.hpp"

#include "path.hpp"
#include "process.hpp"

#ifdef PROG_SYS_WIN

dfa ER PythonExePathGet(CH* path)
{
    ifu (PathEnvRelToAbs(path, L"python.exe") == 0)
        rete(ErrVal::ENV);
    rets;
}
dfa ER PythonStart(cx CH* file, cx CH* args, BO wait)
{
    CH path[PATH_LEN_MAX];
    ifep(PythonExePathGet(path));
    std::wstring _args = L"\"";
    _args += file;
    _args += L"\"";
    if (args != NUL)
    {
        _args += L" ";
        _args += args;
    }
    Proc proc;
    ifep(proc.Start(path, _args.c_str(), NUL));
    if (wait)
    {
        ifep(proc.Wait());
    }
    else
    {
        proc.__Drop();
    }
    rets;
}

#endif
