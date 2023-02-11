#pragma once

enum class ConCol : U1;

dfa ER ConReq();
dfa ER ConColSet(ConCol col);
dfa ER ConColGet(ConCol& col);
dfa ER ConWrite(cx CS* format, ...);
dfa ER ConWriteErr(cx CS* format, ...);
dfa ER ConPause();
