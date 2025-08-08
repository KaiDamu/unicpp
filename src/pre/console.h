#pragma once

enum class ConCol : U1;

dfa ER _ConWriteRaw(cx CS* buf, SI bufLen, BO isInput);

dfa BO ConIsExist();
dfa ER ConReq();
dfa ER ConColSet(ConCol col);
dfa ER ConColGet(ConCol& col);
dfa ER ConWriteRaw(cx CS* form, ...);
dfa ER ConWrite(cx CS* form, ...);
dfa ER ConWriteErr(cx CS* form, ...);
dfa ER ConWait();
