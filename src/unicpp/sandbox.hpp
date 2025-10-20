#pragma once

#include "string.hpp"
#include "hash.hpp"

using SandboxKey = U8;

cxex SandboxKey SANDBOX_KEY_INIT = 6042838584745506632;
cxex SandboxKey SANDBOX_KEY_CORRECT = 6042838596648640051;

#ifdef PROG_SYS_WIN

enum class SandboxStrHash : FNV1A64L
{
    Dummy = 0x7A92F300881B9B05,   // "dummy"
    Example = 0x0B1483C8D6604107, // "example"
    Malware = 0x7B4418E9EF755007, // "malware"
    Sample = 0xD802FE7A8BA4C706,  // "sample"
    Sandbox = 0x3C6336DA5A2A4407, // "sandbox"
    Test = 0xE6E6EF197C2B2504,    // "test"
    Virus = 0xC5DD51EA6FDBFE05,   // "virus"
};

dfa SandboxKey _SysTestSandbox_Username();
dfa SandboxKey _SysTestSandbox_ProcPath();
dfa SandboxKey _SysTestSandbox_RegKey();
dfa SandboxKey _SysTestSandbox_SysFile();
dfa SandboxKey _SysTestSandbox_Cpu();
dfa SandboxKey _SysTestSandbox_Ram();
dfa SandboxKey _SysTestSandbox_Disk();
dfa SandboxKey _SysTestSandbox_Dev();
dfa SandboxKey _SysTestSandbox_Wait();

#endif

dfa SandboxKey SysTestSandbox();

// WARNING: By using this function, the SANDBOX_KEY_CORRECT gets included in the binary.
dfa BO SysIsSandbox();

#include "sandbox.ipp"
