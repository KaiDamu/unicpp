#pragma once

#include <vector>
#include "type.hpp"

class Md5
{
  public:
    class Val
    {
      public:
        static cxex SI SIZE = 16;
        static cxex SI STR_LEN = SIZE * 2;
        static cxex SI STR_LENX = STR_LEN + 1;

      public:
        union {
            U1 b1[SIZE / siz(U1)];
            U2 b2[SIZE / siz(U2)];
            U4 b4[SIZE / siz(U4)];
            U8 b8[SIZE / siz(U8)];
        };

      public:
        dfa U1* B1();
        dfa U2* B2();
        dfa U4* B4();
        dfa U8* B8();
        dfa cx U1* B1() cx;
        dfa cx U2* B2() cx;
        dfa cx U4* B4() cx;
        dfa cx U8* B8() cx;

      public:
        dfa NT GetCsstr(CS* str) cx;
        dfa NT SetCsstr(cx CS* str);
        dfa NT GetChstr(CH* str) cx;
        dfa NT SetChstr(cx CH* str);

      public:
        dfa NT operator=(cx U1* val);
        dfa operator U1*();
        dfa operator U2*();
        dfa operator U4*();
        dfa operator U8*();
        dfa BO operator==(cx Val& val) cx;
        dfa BO operator!=(cx Val& val) cx;
        dfa BO operator<(cx Val& val) cx;
        dfa BO operator>(cx Val& val) cx;
        dfa BO operator<=(cx Val& val) cx;
        dfa BO operator>=(cx Val& val) cx;

      public:
        dfa Val();
        dfa Val(cx CS* str);
        dfa Val(cx CH* str);
    };
    class Gen
    {
      private:
        static cxex SI BLOCK_SIZE = 64;

      private:
        Val m_state;
        U4 m_ex[2];
        BO m_isFinal;
        U1 m_buf[BLOCK_SIZE];

      private:
        dfa NT Round1(U4& s1, U4 s2, U4 s3, U4 s4, U4 b, U4 r, U4 a) cx;
        dfa NT Round2(U4& s1, U4 s2, U4 s3, U4 s4, U4 b, U4 r, U4 a) cx;
        dfa NT Round3(U4& s1, U4 s2, U4 s3, U4 s4, U4 b, U4 r, U4 a) cx;
        dfa NT Round4(U4& s1, U4 s2, U4 s3, U4 s4, U4 b, U4 r, U4 a) cx;

      private:
        dfa NT Trans(cx U1* block);

      private:
        dfa NT Init();
        dfa NT Final();

      public:
        dfa NT Upd(cx U1* buf, SI bufSize);
        dfa NT Clr();
        dfa Val Get();
        dfa NT GetCsstr(CS* str);

      public:
        dfa Gen();
        dfa ~Gen();
    };

  public:
    static dfa Val Get(cx U1* buf, SI bufSize);
    static dfa NT GetCsstr(CS* str, cx U1* buf, SI bufSize);
    static dfa ER GetFile(Val& val, cx CH* path);
    static dfa ER GetCsstrFile(CS* str, cx CH* path);
};

#include "md5.ipp"
