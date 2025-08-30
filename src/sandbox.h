#pragma once

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

dfa SandboxKey _SysTestSandbox_Username()
{
    SandboxKey keyOut = 0xE456E3AE;
    std::wstring user;
    std::wstring fullName;
    ife (ProcUsernameGet(ProcCurHdl(), &user, &fullName))
        ret keyOut + 0xB2B3493B;
    fullName += L"\\";
    fullName += user;
    cx AU fullNameLen = StrToLowcase(&fullName[0]);
    std::vector<CS> fullNameCs(fullNameLen + STR_EX_LEN);
    ChstrToCsstrN(fullNameCs.data(), fullName.c_str(), fullNameLen);
    cx FNV1A64L hashList[7] = {FNV1A64L(SandboxStrHash::Dummy),   FNV1A64L(SandboxStrHash::Example), FNV1A64L(SandboxStrHash::Malware), FNV1A64L(SandboxStrHash::Sample),
                               FNV1A64L(SandboxStrHash::Sandbox), FNV1A64L(SandboxStrHash::Test),    FNV1A64L(SandboxStrHash::Virus)};
    FNV1A64L foundVal;
    cx AU foundPtr = StrFindAnyFnv1a64l(foundVal, fullNameCs.data(), std::span(hashList, 7));
    keyOut += UA(foundPtr); // correct case: (foundPtr == NUL)
    ret keyOut;
}
dfa SandboxKey _SysTestSandbox_ProcPath()
{
    SandboxKey keyOut = 0x30AE1FC8;
    cx AU pathRaw = ProcFilePath();
    ifu (pathRaw == NUL)
        ret keyOut + 0xB9F56492;
    CS path[PATH_LENX_MAX];
    SI i = 0;
    for (; (pathRaw[i] != '\0'); ++i)
        path[i] = ToLowcase(CHToCS(pathRaw[i]));
    path[i] = '\0';
    cx FNV1A64L hashList[5] = {FNV1A64L(SandboxStrHash::Dummy), FNV1A64L(SandboxStrHash::Malware), FNV1A64L(SandboxStrHash::Sample), FNV1A64L(SandboxStrHash::Sandbox),
                               FNV1A64L(SandboxStrHash::Virus)};
    FNV1A64L foundVal;
    cx AU foundPtr = StrFindAnyFnv1a64l(foundVal, path, std::span(hashList, 5));
    keyOut += UA(foundPtr); // correct case: (foundPtr == NUL)
    ret keyOut;
}
dfa SandboxKey _SysTestSandbox_RegKey()
{
    SandboxKey keyOut = 0xD84CF3F1;
    // [key1] L"\\Registry\\Machine\\SOFTWARE\\Oracle\\VirtualBox Guest Additions"
    // [key2] L"\\Registry\\Machine\\HARDWARE\\ACPI\\DSDT\\VBOX__"
    // [key3] L"\\Registry\\Machine\\HARDWARE\\ACPI\\FADT\\VBOX__"
    // [key4] L"\\Registry\\Machine\\HARDWARE\\ACPI\\RSDT\\VBOX__"
    StrObfuscated<122> key1({179, 0,   175, 254, 222, 4,   222, 2,   222, 248, 198, 246, 199, 252, 203, 250, 230, 16,  193, 14,  214, 20,  240, 18,  232, 8,   237, 6,   242, 12,  239,
                             10,  186, 224, 145, 222, 148, 228, 166, 226, 149, 216, 129, 214, 136, 220, 152, 218, 161, 240, 168, 238, 167, 244, 182, 242, 185, 232, 196, 230, 204, 236,
                             205, 234, 26,  64,  241, 62,  241, 68,  40,  66,  9,   56,  1,   54,  6,   60,  24,  58,  51,  80,  15,  78,  56,  84,  41,  82,  103, 72,  2,   70,  22,
                             76,  36,  74,  240, 32,  233, 30,  67,  36,  224, 34,  243, 24,  241, 22,  2,   28,  229, 26,  22,  48,  18,  46,  25,  52,  2,   50,  39,  40});
    StrObfuscated<88> key2({179, 0,   175, 254, 222, 4,   222, 2,   222, 248, 198, 246, 199, 252, 203, 250, 230, 16,  193, 14,  214, 20,  240, 18,  232, 8,   237, 6,   242, 12,
                            239, 10,  186, 224, 145, 222, 171, 228, 160, 226, 137, 216, 145, 214, 136, 220, 152, 218, 161, 240, 168, 238, 167, 244, 176, 242, 168, 232, 149, 230,
                            178, 236, 157, 234, 251, 64,  238, 62,  255, 68,  237, 66,  235, 56,  227, 54,  253, 60,  254, 58,  7,   80,  2,   78,  8,   84,  81,  82});
    StrObfuscated<88> key3({179, 0,   175, 254, 222, 4,   222, 2,   222, 248, 198, 246, 199, 252, 203, 250, 230, 16,  193, 14,  214, 20,  240, 18,  232, 8,   237, 6,   242, 12,
                            239, 10,  186, 224, 145, 222, 171, 228, 160, 226, 137, 216, 145, 214, 136, 220, 152, 218, 161, 240, 168, 238, 167, 244, 176, 242, 168, 232, 149, 230,
                            178, 236, 157, 234, 253, 64,  252, 62,  255, 68,  237, 66,  235, 56,  227, 54,  253, 60,  254, 58,  7,   80,  2,   78,  8,   84,  81,  82});
    StrObfuscated<88> key4({179, 0,   175, 254, 222, 4,   222, 2,   222, 248, 198, 246, 199, 252, 203, 250, 230, 16,  193, 14,  214, 20,  240, 18,  232, 8,   237, 6,   242, 12,
                            239, 10,  186, 224, 145, 222, 171, 228, 160, 226, 137, 216, 145, 214, 136, 220, 152, 218, 161, 240, 168, 238, 167, 244, 176, 242, 168, 232, 149, 230,
                            178, 236, 157, 234, 241, 64,  238, 62,  255, 68,  237, 66,  235, 56,  227, 54,  253, 60,  254, 58,  7,   80,  2,   78,  8,   84,  81,  82});
    if (RegKeyIsExist(key1.Chstr(YES)))
        keyOut += 0x447B065D;
    if (RegKeyIsExist(key2.Chstr(YES)))
        keyOut += 0xB12B388F;
    if (RegKeyIsExist(key3.Chstr(YES)))
        keyOut += 0x828B9966;
    if (RegKeyIsExist(key4.Chstr(YES)))
        keyOut += 0x134A518D;
    ret keyOut;
}
dfa SandboxKey _SysTestSandbox_SysFile()
{
    SandboxKey keyOut = 0xD8296F84;
    // [strKey] L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"
    // [strVal] L"PathName"
    // [strFile1] L"\\System32\\drivers\\vmmouse.sys"
    // [strFile2] L"\\System32\\drivers\\vmhgfs.sys"
    StrObfuscated<126> strKey({179, 0,   175, 254, 222, 4,   222, 2,   222, 248, 198, 246, 199, 252, 203, 250, 230, 16,  193, 14,  214, 20,  240, 18,  232, 8,   237, 6,   242, 12,  239, 10,
                               186, 224, 145, 222, 148, 228, 166, 226, 149, 216, 129, 214, 136, 220, 152, 218, 161, 240, 168, 238, 167, 244, 180, 242, 206, 232, 198, 230, 189, 236, 206, 234,
                               16,  64,  34,  62,  33,  68,  13,  66,  235, 56,  226, 54,  34,  60,  31,  58,  43,  80,  50,  78,  32,  84,  34,  82,  103, 72,  11,  70,  247, 76,  253, 74,
                               224, 32,  232, 30,  245, 36,  243, 34,  242, 24,  251, 22,  231, 28,  199, 26,  10,  48,  255, 46,  4,   52,  24,  50,  12,  40,  11,  38,  43,  44});
    StrObfuscated<18> strVal({175, 0, 220, 254, 207, 4, 233, 2, 189, 248, 212, 246, 222, 252, 212, 250, 15, 16});
    StrObfuscated<60> strFile1({179, 0,  174, 254, 218, 4,   210, 2,   195, 248, 208, 246, 222, 252, 10,  250, 33,  16,  193, 14,  239, 20,  227, 18,  238, 8,   211, 6,   230, 12,
                                219, 10, 176, 224, 145, 222, 177, 228, 196, 226, 186, 216, 186, 214, 166, 220, 170, 218, 202, 240, 19,  238, 196, 244, 200, 242, 184, 232, 229, 230});
    StrObfuscated<58> strFile2({179, 0,   174, 254, 218, 4,   210, 2,   195, 248, 208, 246, 222, 252, 10,  250, 33,  16,  193, 14, 239, 20,  227, 18,  238, 8,   211, 6,   230,
                                12,  219, 10,  176, 224, 145, 222, 177, 228, 196, 226, 191, 216, 178, 214, 185, 220, 170, 218, 21, 240, 190, 238, 202, 244, 194, 242, 231, 232});
    std::wstring winDir;
    ife (RegKeyGetStr(winDir, strKey.Chstr(YES), strVal.Chstr(YES)))
        keyOut += 0x205CEF38;
    if (FileIsExist((winDir + strFile1.Chstr(YES)).c_str()))
        keyOut += 0xAF654A3F;
    if (FileIsExist((winDir + strFile2.Chstr(YES)).c_str()))
        keyOut += 0x1472A8C1;
    ret keyOut;
}
dfa SandboxKey _SysTestSandbox_Cpu()
{
    // TODO
    ret 0;
}
dfa SandboxKey _SysTestSandbox_Ram()
{
    // TODO
    ret 0;
}
dfa SandboxKey _SysTestSandbox_Disk()
{
    // TODO
    ret 0;
}
dfa SandboxKey _SysTestSandbox_Dev()
{
    // TODO
    ret 0;
}
dfa SandboxKey _SysTestSandbox_Wait()
{
    // TODO
    ret 0;
}

#endif

dfa SandboxKey SysTestSandbox()
{
#ifdef PROG_SYS_WIN
    SandboxKey key = SANDBOX_KEY_INIT;
    key += _SysTestSandbox_Username();
    key += _SysTestSandbox_ProcPath();
    key += _SysTestSandbox_RegKey();
    key += _SysTestSandbox_SysFile();
    key += _SysTestSandbox_Cpu();
    key += _SysTestSandbox_Ram();
    key += _SysTestSandbox_Disk();
    key += _SysTestSandbox_Dev();
    key += _SysTestSandbox_Wait();
    ret key;
#else
    ret SANDBOX_KEY_CORRECT;
#endif
}

// WARNING: By using this function, the SANDBOX_KEY_CORRECT gets included in the binary.
dfa BO SysIsSandbox()
{
    volatile SandboxKey key = SysTestSandbox();
    ret BO(key ^ SANDBOX_KEY_CORRECT);
}
