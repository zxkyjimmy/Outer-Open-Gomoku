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
bool canwin(uint64_t line)
{
    for(uint64_t i = 0; i < 9; i++)
    {
        if(empty(line, i))
        {
            if(state[getfeat(line, i)] & 0xC00)
            {
                return true;
            }
        }
    }
    return false;
}
uint64_t Cdefense(uint64_t f)
{
    uint64_t line = getline(f), result = 0;
    if(state[f] & 0x300) //D4, L3
    {
        for(uint64_t i = 0; i < 9; i++)
        {
            if(empty(line, i))
            {
                if(!canwin(line ^ (0x1000000000 << i)))
                {
                    result |= 1 << i;
                }
            }
        }
    }
    else if(state[f] & 0x400) //L4
    {
        for(uint64_t i = 0; i < 9; i++)
        {
            if(empty(line, i))
            {
                if(state[getfeat(line, i)] & 0x800)
                {
                    result |= 1 << i;
                }
            }
        }
    }
    return _pext_u64(result, 0x1EF);
}
uint64_t cnum(uint64_t c)
{
    constexpr uint64_t czone[32] =
    {
          0,  1,  2,  4,  8, 16, 17, 18, 19, 20, 21, 24, 25, 32, 34, 36,
         38, 40, 42, 50, 64, 68, 72, 76, 84,100,128,136,152,168,200,
    };
    for(uint64_t i = 0; i < 31; i++)
    {
        if(czone[i] == c)
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
        uint64_t c = Cdefense(i);
        uint64_t num = cnum(c);
        state_new[i] = (state[i] << 8) | num;
    }
    cout << "static constexpr uint64_t state[65536] =" << endl;
    cout << "{" << endl;
    for(int i = 0; i < 4096; i++)
    {
        cout << "    ";
        for(int j = 0; j < 16; j++)
        {
            cout << "0x" << hex << setw(5) << setfill('0') << state_new[i*16+j] << ",";
        }
        cout << endl;
    }
    cout << "};" << endl;
    return 0;
}
