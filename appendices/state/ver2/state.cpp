#include <iostream>
#include <immintrin.h>
#include "old_state.h"
using namespace std;
uint64_t new2old(uint64_t index)
{
    uint64_t result = 0, sample;
    for(int i = 0; i < 8; i++)
    {
        sample = _pext_u64(index, 0x101 << i);
        if(sample == 0) // wall as opp
        {
            result |= 0x100 << i;
        }
        else if(sample == 1) // opp
        {
            result |= 0x100 << i;
        }
        else if(sample == 2) // my
        {
            result |= 0x001 << i;
        }
        else if(sample == 3) // empty
        {
            ;//nothing
        }
        else
        {
            cout << "WTF!??????" << endl;
        }
    }
    return result;
}
constexpr uint64_t state_size = 65536;
uint64_t newstate[state_size] = {0};
int main()
{
    for(uint64_t i = 0; i < state_size; i++)
    {
        newstate[i] = state[new2old(i)];
    }
    cout << "static constexpr uint64_t state[65536] =" << endl;
    cout << "{" << endl;
    for(int i = 0, j = 0; i < 65536; i++, j++)
    {
        if(j == 0) cout << "    ";
        cout << newstate[i];
        if(i != state_size - 1)cout << ",";
        if(j == 63){cout << endl; j = -1;}
    }
    cout << "};" << endl;

    for(uint64_t index = 0; index < state_size; index++)
    {
        for(int i = 0; i < 4; i++)
        {
            switch(_pext_u64(index, 0x101 << i))
            {
            case 0:
                cout << "w";
                break;
            case 1:
                cout << "0";
                break;
            case 2:
                cout << "1";
                break;
            case 3:
                cout << ".";
                break;
            }
        }
        cout << "1";
        for(int i = 4; i < 8; i++)
        {
            switch(_pext_u64(index, 0x101 << i))
            {
            case 0:
                cout << "w";
                break;
            case 1:
                cout << "0";
                break;
            case 2:
                cout << "1";
                break;
            case 3:
                cout << ".";
                break;
            }
        }
        cout << " : ";
        if(newstate[index] == 8)cout << "L5";
        else if(newstate[index] == 4)cout << "L4";
        else if(newstate[index] == 2)cout << "D4";
        else if(newstate[index] == 1)cout << "L3";
        else if(newstate[index] == 0);
        else cout << "WTF?!";
        cout << endl;
    }
    return 0;
}
