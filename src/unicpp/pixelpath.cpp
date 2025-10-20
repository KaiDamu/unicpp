#include "pixelpath.hpp"

dfa SI PixelPath::GetIOfsByDir(Dir dir, SI w)
{
    switch (dir)
    {
    case Dir::UL:
        ret -w - 1;
    case Dir::U:
        ret -w;
    case Dir::UR:
        ret -w + 1;
    case Dir::L:
        ret -1;
    case Dir::R:
        ret 1;
    case Dir::DL:
        ret w - 1;
    case Dir::D:
        ret w;
    case Dir::DR:
        ret w + 1;
    default:
        ret 0;
    }
}
dfa PixelPath::Dir PixelPath::GetBackDir(Dir dir)
{
    cx Dir backDir[] = {
        Dir::DR, // UL
        Dir::D,  // U
        Dir::DL, // UR
        Dir::R,  // L
        Dir::L,  // R
        Dir::UR, // DL
        Dir::U,  // D
        Dir::UL  // DR
    };
    ret backDir[SI(dir)];
}
dfa U1 PixelPath::DirLenToMoves(SI len)
{
    cxex U1 tab[] = {0b00, 0b00, 0b01, 0b10, 0b0101, 0b1001, 0b0110, 0b1010, 0b010101, 0b100101, 0b011001, 0b101001, 0b010110, 0b100110, 0b011010, 0b11101010};
    ret tab[len > 15 ? 15 : len];
}
dfa SI PixelPath::MovesToDirLen(U1 moveHash)
{
    cxex U1 tab[] = {0, 2, 3, 0, 4, 6, 0, 5, 7, 0, 0, 0, 0, 8, 12, 0, 10, 14, 0, 0, 0, 0, 9, 13, 0, 11, DIR_LEN_VAR};
    ret tab[moveHash];
}
