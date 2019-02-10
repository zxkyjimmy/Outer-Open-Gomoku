#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>
#include <immintrin.h>
#include "old_state.h"
using namespace std;
uint64_t getline(uint64_t index)
{
    return _pdep_u64(index, 0x1EF000001EF0) | 0x10000000000;
}
uint64_t getfeat(uint64_t line, uint64_t offset)
{
    return _pext_u64(line, 0x1EF000001EFull << offset);
}
bool empty(uint64_t line, uint64_t offset)
{
    return _pext_u64(line, 0x1000000010 << offset) == 3;
}
uint64_t threat(uint64_t line)
{
    uint64_t old_line = line | 0x100, old_pattern = 0, pattern = 0;
    for(uint64_t i = 0; i < 4; i++)
    {
        if(empty(old_line, i) && state[getfeat(old_line, i)])
        {
            old_pattern |= 0x1ull << i;
        }
        if(empty(line, i) && state[getfeat(line, i)])
        {
            pattern |= 0x1ull << i;
        }
        if(empty(old_line, i+5) && state[getfeat(old_line, i+5)])
        {
            old_pattern |= 0x1ull << (i+4);
        }
        if(empty(line, i+5) && state[getfeat(line, i+5)])
        {
            pattern |= 0x1ull << (i+4);
        }
    }
    return pattern & ~old_pattern;
}
uint64_t tnum(uint64_t t)
{
    constexpr uint64_t tzone[64] =
    {
          0,  1,  2,  3,  4,  5,  6,  8,  9, 10, 12, 16, 17, 18, 20, 22,
         24, 26, 28, 32, 33, 34, 36, 40, 44, 48, 49, 52, 54, 56, 58, 60,
         64, 65, 66, 68, 70, 72, 80, 81, 88, 90, 92, 96, 97, 98,104,108,
        128,129,130,132,134,136,138,140,144,160,192
    };
    for(uint64_t i = 0; i < 59; i++)
    {
        if(tzone[i] == t)
        {
            return i;
        }
    }
    cout << "R u kiDdiNg mE?!" << endl;
    return 666;
}
uint64_t state_new[65536] = {0};
int main()
{
    for(uint64_t i = 0; i < 65536; i++)
    {
        uint64_t line = getline(i);
        uint64_t t = threat(line);
        uint64_t num = tnum(t);
        state_new[i] = (state[i] << 8) | num;
    }
    cout << "static constexpr uint64_t state_Tzone[65536] =" << endl;
    cout << "{" << endl;
    for(int i = 0; i < 2048; i++)
    {
        cout << "    ";
        for(int j = 0; j < 32; j++)
        {
            cout << "0x" << hex << setw(3) << setfill('0') << state_new[i*32+j] << ",";
        }
        cout << endl;
    }
    cout << "};" << endl;
    return 0;
}
