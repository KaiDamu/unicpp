#pragma once

// performance: up to 620 MB/s
class Md5
{
  public:
    class Val
    {
      public:
        static constexpr SI SIZE = 16;
        static constexpr SI STR_LEN = SIZE * 2;
        static constexpr SI STR_LENX = STR_LEN + 1;

      public:
        union {
            U1 b1[SIZE / siz(U1)];
            U2 b2[SIZE / siz(U2)];
            U4 b4[SIZE / siz(U4)];
            U8 b8[SIZE / siz(U8)];
        };

      public:
        dfa U1* B1()
        {
            ret b1;
        }
        dfa U2* B2()
        {
            ret b2;
        }
        dfa U4* B4()
        {
            ret b4;
        }
        dfa U8* B8()
        {
            ret b8;
        }
        dfa cx U1* B1() cx
        {
            ret b1;
        }
        dfa cx U2* B2() cx
        {
            ret b2;
        }
        dfa cx U4* B4() cx
        {
            ret b4;
        }
        dfa cx U8* B8() cx
        {
            ret b8;
        }

      public:
        dfa NT GetCsstr(CS* str) cx
        {
            CsstrSetForm(str, "%08x%08x%08x%08x", RevByte(b4[0]), RevByte(b4[1]), RevByte(b4[2]), RevByte(b4[3]));
        }
        dfa NT SetCsstr(cx CS* str)
        {
            CsstrGetForm(str, "%08x%08x%08x%08x", &b4[0], &b4[1], &b4[2], &b4[3]);
            b4[0] = RevByte(b4[0]);
            b4[1] = RevByte(b4[1]);
            b4[2] = RevByte(b4[2]);
            b4[3] = RevByte(b4[3]);
        }
        dfa NT GetChstr(CH* str) cx
        {
            ChstrSetForm(str, L"%08x%08x%08x%08x", RevByte(b4[0]), RevByte(b4[1]), RevByte(b4[2]), RevByte(b4[3]));
        }
        dfa NT SetChstr(cx CH* str)
        {
            ChstrGetForm(str, L"%08x%08x%08x%08x", &b4[0], &b4[1], &b4[2], &b4[3]);
            b4[0] = RevByte(b4[0]);
            b4[1] = RevByte(b4[1]);
            b4[2] = RevByte(b4[2]);
            b4[3] = RevByte(b4[3]);
        }

      public:
        // dfa NT operator = (cx Val& val) { MemCpy(tx, val.B1(), siz(*tx)); }
        dfa NT operator=(cx U1* val)
        {
            MemCpy(tx, val, siz(*tx));
        }
        dfa operator U1*()
        {
            ret b1;
        }
        dfa operator U2*()
        {
            ret b2;
        }
        dfa operator U4*()
        {
            ret b4;
        }
        dfa operator U8*()
        {
            ret b8;
        }
        dfa BO operator==(cx Val& val) cx
        {
            ret MemCmp(tx, val.B1(), siz(*tx)) == 0;
        }
        dfa BO operator!=(cx Val& val) cx
        {
            ret MemCmp(tx, val.B1(), siz(*tx)) != 0;
        }
        dfa BO operator<(cx Val& val) cx
        {
            ret MemCmp(tx, val.B1(), siz(*tx)) < 0;
        }
        dfa BO operator>(cx Val& val) cx
        {
            ret MemCmp(tx, val.B1(), siz(*tx)) > 0;
        }
        dfa BO operator<=(cx Val& val) cx
        {
            ret MemCmp(tx, val.B1(), siz(*tx)) <= 0;
        }
        dfa BO operator>=(cx Val& val) cx
        {
            ret MemCmp(tx, val.B1(), siz(*tx)) >= 0;
        }

      public:
        dfa Val()
        {
            MemSet(tx, 0, siz(*tx));
        }
        dfa Val(cx CS* str)
        {
            tx->SetCsstr(str);
        }
        dfa Val(cx CH* str)
        {
            tx->SetChstr(str);
        }
    };
    class Gen
    {
      private:
        static constexpr SI BLOCK_SIZE = 64;

      private:
        Val m_state;
        U4 m_ex[2];
        BO m_isFinal;
        U1 m_buf[BLOCK_SIZE];

      private:
        dfa NT Round1(U4& s1, U4 s2, U4 s3, U4 s4, U4 b, U4 r, U4 a) cx
        {
            s1 += ((s2 & s3) | (~s2 & s4)) + b + a;
            s1 = RotL(s1, r) + s2;
        }
        dfa NT Round2(U4& s1, U4 s2, U4 s3, U4 s4, U4 b, U4 r, U4 a) cx
        {
            s1 += ((s2 & s4) | (s3 & ~s4)) + b + a;
            s1 = RotL(s1, r) + s2;
        }
        dfa NT Round3(U4& s1, U4 s2, U4 s3, U4 s4, U4 b, U4 r, U4 a) cx
        {
            s1 += (s2 ^ s3 ^ s4) + b + a;
            s1 = RotL(s1, r) + s2;
        }
        dfa NT Round4(U4& s1, U4 s2, U4 s3, U4 s4, U4 b, U4 r, U4 a) cx
        {
            s1 += (s3 ^ (s2 | ~s4)) + b + a;
            s1 = RotL(s1, r) + s2;
        }

      private:
        dfa NT Trans(cx U1* block)
        {
            AU b = (cx U4*)block;
            AU s = m_state;
            // looks bad, but it's the fastest way to do it & tested
            tx->Round1(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[0], 7, 0xd76aa478);
            tx->Round1(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[1], 12, 0xe8c7b756);
            tx->Round1(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[2], 17, 0x242070db);
            tx->Round1(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[3], 22, 0xc1bdceee);
            tx->Round1(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[4], 7, 0xf57c0faf);
            tx->Round1(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[5], 12, 0x4787c62a);
            tx->Round1(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[6], 17, 0xa8304613);
            tx->Round1(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[7], 22, 0xfd469501);
            tx->Round1(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[8], 7, 0x698098d8);
            tx->Round1(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[9], 12, 0x8b44f7af);
            tx->Round1(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[10], 17, 0xffff5bb1);
            tx->Round1(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[11], 22, 0x895cd7be);
            tx->Round1(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[12], 7, 0x6b901122);
            tx->Round1(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[13], 12, 0xfd987193);
            tx->Round1(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[14], 17, 0xa679438e);
            tx->Round1(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[15], 22, 0x49b40821);
            tx->Round2(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[1], 5, 0xf61e2562);
            tx->Round2(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[6], 9, 0xc040b340);
            tx->Round2(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[11], 14, 0x265e5a51);
            tx->Round2(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[0], 20, 0xe9b6c7aa);
            tx->Round2(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[5], 5, 0xd62f105d);
            tx->Round2(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[10], 9, 0x02441453);
            tx->Round2(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[15], 14, 0xd8a1e681);
            tx->Round2(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[4], 20, 0xe7d3fbc8);
            tx->Round2(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[9], 5, 0x21e1cde6);
            tx->Round2(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[14], 9, 0xc33707d6);
            tx->Round2(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[3], 14, 0xf4d50d87);
            tx->Round2(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[8], 20, 0x455a14ed);
            tx->Round2(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[13], 5, 0xa9e3e905);
            tx->Round2(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[2], 9, 0xfcefa3f8);
            tx->Round2(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[7], 14, 0x676f02d9);
            tx->Round2(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[12], 20, 0x8d2a4c8a);
            tx->Round3(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[5], 4, 0xfffa3942);
            tx->Round3(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[8], 11, 0x8771f681);
            tx->Round3(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[11], 16, 0x6d9d6122);
            tx->Round3(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[14], 23, 0xfde5380c);
            tx->Round3(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[1], 4, 0xa4beea44);
            tx->Round3(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[4], 11, 0x4bdecfa9);
            tx->Round3(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[7], 16, 0xf6bb4b60);
            tx->Round3(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[10], 23, 0xbebfbc70);
            tx->Round3(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[13], 4, 0x289b7ec6);
            tx->Round3(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[0], 11, 0xeaa127fa);
            tx->Round3(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[3], 16, 0xd4ef3085);
            tx->Round3(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[6], 23, 0x04881d05);
            tx->Round3(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[9], 4, 0xd9d4d039);
            tx->Round3(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[12], 11, 0xe6db99e5);
            tx->Round3(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[15], 16, 0x1fa27cf8);
            tx->Round3(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[2], 23, 0xc4ac5665);
            tx->Round4(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[0], 6, 0xf4292244);
            tx->Round4(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[7], 10, 0x432aff97);
            tx->Round4(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[14], 15, 0xab9423a7);
            tx->Round4(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[5], 21, 0xfc93a039);
            tx->Round4(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[12], 6, 0x655b59c3);
            tx->Round4(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[3], 10, 0x8f0ccc92);
            tx->Round4(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[10], 15, 0xffeff47d);
            tx->Round4(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[1], 21, 0x85845dd1);
            tx->Round4(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[8], 6, 0x6fa87e4f);
            tx->Round4(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[15], 10, 0xfe2ce6e0);
            tx->Round4(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[6], 15, 0xa3014314);
            tx->Round4(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[13], 21, 0x4e0811a1);
            tx->Round4(s.b4[0], s.b4[1], s.b4[2], s.b4[3], b[4], 6, 0xf7537e82);
            tx->Round4(s.b4[3], s.b4[0], s.b4[1], s.b4[2], b[11], 10, 0xbd3af235);
            tx->Round4(s.b4[2], s.b4[3], s.b4[0], s.b4[1], b[2], 15, 0x2ad7d2bb);
            tx->Round4(s.b4[1], s.b4[2], s.b4[3], s.b4[0], b[9], 21, 0xeb86d391);
            m_state.b4[0] += s.b4[0];
            m_state.b4[1] += s.b4[1];
            m_state.b4[2] += s.b4[2];
            m_state.b4[3] += s.b4[3];
        }

      private:
        dfa NT Init()
        {
            m_state.b4[0] = 0x67452301;
            m_state.b4[1] = 0xEFCDAB89;
            m_state.b4[2] = 0x98BADCFE;
            m_state.b4[3] = 0x10325476;
            m_ex[0] = 0;
            m_ex[1] = 0;
            m_isFinal = NO;
            MemSet(m_buf, 0, siz(m_buf));
        }
        dfa NT Final()
        {
            if (m_isFinal)
                ret;
            U1 ex[siz(m_ex)];
            MemCpy(ex, m_ex, siz(m_ex));
            cx SI ofs = (m_ex[0] >> 3) & 0x3f;
            cx SI padLen = (ofs < 56) ? (56 - ofs) : (120 - ofs);
            cx U1 pad[BLOCK_SIZE] = {0x80};
            tx->Upd(pad, padLen);
            tx->Upd(ex, siz(ex));
            m_isFinal = YES;
        }

      public:
        dfa NT Upd(cx U1* buf, SI bufSize)
        {
            ifu (bufSize == 0)
                ret;
            if (m_isFinal)
                tx->Init();
            SI i = 0;
            SI j = (m_ex[0] >> 3) & 0x3F;
            cx SI partSize = BLOCK_SIZE - j;
            m_ex[0] += U4(bufSize << 3);
            if (m_ex[0] < (bufSize << 3))
                ++m_ex[1];
            m_ex[1] += bufSize >> (sizb(TO(bufSize)) - 3);
            if (bufSize >= partSize)
            {
                MemCpy(&m_buf[j], buf, partSize);
                tx->Trans(m_buf);
                for (i = partSize; i + BLOCK_SIZE <= bufSize; i += BLOCK_SIZE)
                {
                    tx->Trans(&buf[i]);
                }
                j = 0;
            }
            MemCpy(&m_buf[j], &buf[i], bufSize - i);
        }
        dfa NT Clr()
        {
            tx->Init();
        }
        dfa Val Get()
        {
            tx->Final();
            ret m_state;
        }
        dfa NT GetCsstr(CS* str)
        {
            tx->Get().GetCsstr(str);
        }

      public:
        dfa Gen()
        {
            tx->Init();
        }
        dfa ~Gen()
        {
        }
    };

  public:
    static dfa Val Get(cx U1* buf, SI bufSize)
    {
        Gen gen;
        gen.Upd(buf, bufSize);
        ret gen.Get();
    }
    static dfa NT GetCsstr(CS* str, cx U1* buf, SI bufSize)
    {
        cx Val val = Md5::Get(buf, bufSize);
        val.GetCsstr(str);
    }
    static dfa ER GetFile(Val& val, cx CH* path)
    {
        File f;
        ife (f.OpenRead(path))
            retep;
        Md5::Gen md5;
        Arr<U1> buf(FILE_READ_SIZE_HIGH);
        SI readSize = 0;
        do
        {
            ife (f.Read(buf, buf.Cap(), readSize))
                retep;
            md5.Upd(buf, readSize);
        } while (readSize == buf.Cap());
        ife (f.Close())
            retep;
        val = md5.Get();
        rets;
    }
    static dfa ER GetCsstrFile(CS* str, cx CH* path)
    {
        Val val;
        ife (Md5::GetFile(val, path))
            retep;
        val.GetCsstr(str);
        rets;
    }
};

tpl0 struct std::hash<Md5::Val>
{
    std::size_t operator()(cx Md5::Val& val) cx noex
    {
        ret std::size_t(val.b8[0] ^ val.b8[1]);
    }
};
