#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include <immintrin.h>
#include "const.h"

class BitBoard
{
public:
    //data
    uint64_t m[4] = {0x7FFF7FFF7FFF7FFF, 0x7FFF7FFF7FFF7FFF, 0x7FFF7FFF7FFF7FFF, 0x7FFF7FFF7FFF0000};
    //constructor
    BitBoard() = default;
    BitBoard(uint64_t a, uint64_t b, uint64_t c, uint64_t d):m{a,b,c,d}{}
    //operator
    inline void init()
    {
        m[0] = 0x7FFF7FFF7FFF7FFF; m[1] = 0x7FFF7FFF7FFF7FFF; m[2] = 0x7FFF7FFF7FFF7FFF; m[3] = 0x7FFF7FFF7FFF0000;
    }
    inline void initZero()
    {
        m[0] = 0; m[1] = 0; m[2] = 0; m[3] = 0;
    }
    inline operator bool() const {return m[0]|m[1]|m[2]|m[3];}
    inline const BitBoard operator&(BitBoard b) const
    {
        return BitBoard(m[0]&b.m[0],m[1]&b.m[1],m[2]&b.m[2],m[3]&b.m[3]);
    }
    inline const BitBoard operator|(BitBoard b) const
    {
        return BitBoard(m[0]|b.m[0],m[1]|b.m[1],m[2]|b.m[2],m[3]|b.m[3]);
    }
    inline const BitBoard operator^(BitBoard b) const
    {
        return BitBoard(m[0]^b.m[0],m[1]^b.m[1],m[2]^b.m[2],m[3]^b.m[3]);
    }
    inline const BitBoard operator~() const
    {
        return BitBoard(~m[0],~m[1],~m[2],~m[3]);
    }
    //function
    inline uint64_t ls1b()
    {
        uint64_t result = 0;
        if(m[0])
        {
            result = _lzcnt_u64(_blsi_u64(m[0]));
            m[0] = _blsr_u64(m[0]);
        }
        else if(m[1])
        {
            result = _lzcnt_u64(_blsi_u64(m[1]))|0x40;
            m[1] = _blsr_u64(m[1]);
        }
        else if(m[2])
        {
            result = _lzcnt_u64(_blsi_u64(m[2]))|0x80;
            m[2] = _blsr_u64(m[2]);
        }
        else if(m[3])
        {
            result = _lzcnt_u64(_blsi_u64(m[3]))|0xC0;
            m[3] = _blsr_u64(m[3]);
        }
        return result;
    }
    inline void append(const uint64_t pos)
    {
        m[pos>>6] ^= 0x8000000000000000ull >> (pos&0x3f);
    }
    /*
    inline void consider(const uint64_t pos)
    {
        m[0] |= Pozidriv2_a[pos];
        m[1] |= Pozidriv2_b[pos];
        m[2] |= Pozidriv2_c[pos];
        m[3] |= Pozidriv2_d[pos];
    }*/
    inline const BitBoard mind(uint64_t pos) const
    {
        return BitBoard(m[0]|Pozidriv2_a[pos],m[1]|Pozidriv2_b[pos],m[2]|Pozidriv2_c[pos],m[3]|Pozidriv2_d[pos]);
    }
    //for debug
    void print()const
    {
        #include <iostream>
        using namespace std;
        cout << hex << m[0] << endl;
        for(int i = 0; i < 64; i++)
        {
            cout << bool(m[0] & (0x8000000000000000ull >> i));
            if(i % 16 == 15)cout << endl;
        }
        for(int i = 0; i < 64; i++)
        {
            cout << bool(m[1] & (0x8000000000000000ull >> i));
            if(i % 16 == 15)cout << endl;
        }
        for(int i = 0; i < 64; i++)
        {
            cout << bool(m[2] & (0x8000000000000000ull >> i));
            if(i % 16 == 15)cout << endl;
        }
        for(int i = 0; i < 64; i++)
        {
            cout << bool(m[3] & (0x8000000000000000ull >> i));
            if(i % 16 == 15)cout << endl;
        }
    }
};

inline uint64_t feature(const BitBoard &my, const BitBoard &opp, const uint64_t pos)
{
    uint64_t mask = Pozidriv4_a[pos];
    uint64_t my_feature  = _pext_u64( my.m[0], mask);
    uint64_t opp_feature = _pext_u64(opp.m[0], mask);
    mask = Pozidriv4_b[pos];
    uint64_t offset = _mm_popcnt_u64(mask);
    my_feature  <<= offset;
    opp_feature <<= offset;
    my_feature  |= _pext_u64( my.m[1], mask);
    opp_feature |= _pext_u64(opp.m[1], mask);
    mask = Pozidriv4_c[pos];
    offset = _mm_popcnt_u64(mask);
    my_feature  <<= offset;
    opp_feature <<= offset;
    my_feature  |= _pext_u64( my.m[2], mask);
    opp_feature |= _pext_u64(opp.m[2], mask);
    mask = Pozidriv4_d[pos];
    offset = _mm_popcnt_u64(mask);
    my_feature  <<= offset;
    opp_feature <<= offset;
    my_feature  |= _pext_u64( my.m[3], mask);
    opp_feature |= _pext_u64(opp.m[3], mask);
    mask = Pozidriv_pdep[pos];
    my_feature  = _pdep_u64(my_feature, mask);
    opp_feature = _pdep_u64(opp_feature, mask);
    return my_feature | (opp_feature << 32);
}
#endif // BITBOARD_H_INCLUDED
