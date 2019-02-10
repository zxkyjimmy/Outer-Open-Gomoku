#include <iostream>
#include <iomanip>
#include <immintrin.h>
#define Debug 1
using namespace std;
uint8_t check_state(uint64_t line)
{
    uint8_t result = 0;
    uint64_t sample, mask;
    /**L5**/
    sample = 0x1F0;//XXXXX
    mask = 0x1F0000001F0;
    for(int i = 0; i < 5; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 8;
        }
    }
    if(result)return result;
    /**L4**/
    sample = 0x1E0;//.XXXX.
    mask = 0x3F0000003F0;
    for(int i = 0; i < 4; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 4;
        }
    }
    if(result)return result;
    sample = 0x5D0;//X.XXX.X
    mask = 0x7F0000007F0;
    for(int i = 0; i < 3; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 4;
        }
    }
    if(result)return result;
    sample = 0xDB0;//XX.XX.XX
    mask = 0xFF000000FF0;
    for(int i = 0; i < 2; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 4;
        }
    }
    if(result)return result;
    sample = 0x1D70;//XXX.X.XXX
    mask = 0x1FF000001FF0;
    for(int i = 0; i < 1; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 4;
        }
    }
    if(result)return result;
    /**D4**/
    sample = 0xF0;//.XXXX
    mask = 0x1F0000001F0;
    for(int i = 0; i < 5; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 2;
        }
    }
    if(result)return result;
    sample = 0x170;//X.XXX
    mask = 0x1F0000001F0;
    for(int i = 0; i < 5; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 2;
        }
    }
    if(result)return result;
    sample = 0x1B0;//XX.XX
    mask = 0x1F0000001F0;
    for(int i = 0; i < 5; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 2;
        }
    }
    if(result)return result;
    sample = 0x1D0;//XXX.X
    mask = 0x1F0000001F0;
    for(int i = 0; i < 5; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 2;
        }
    }
    if(result)return result;
    sample = 0x1E0;//XXXX.
    mask = 0x1F0000001F0;
    for(int i = 0; i < 5; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 2;
        }
    }
    if(result)return result;
    /**L3**/
    sample = 0xE0;//..XXX.
    mask = 0x3F0000003F0;
    for(int i = 0; i < 4; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 1;
        }
    }
    if(result)return result;
    sample = 0x1C0;//.XXX..
    mask = 0x3F0000003F0;
    for(int i = 0; i < 4; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 1;
        }
    }
    if(result)return result;
    sample = 0x160;//.X.XX.
    mask = 0x3F0000003F0;
    for(int i = 0; i < 4; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 1;
        }
    }
    if(result)return result;
    sample = 0x1A0;//.XX.X.
    mask = 0x3F0000003F0;
    for(int i = 0; i < 4; i++)
    {
        if((line & (mask << i)) == (sample << i))
        {
            result = 1;
        }
    }
    if(result)return result;
    return 0;
}
uint8_t state[65536] = {0};
int main()
{
    int count = 0;
    for(int i = 0; i < 65536; i++)
    {
        if((i >> 8) & i)continue;
        count++;
        uint64_t line = _pdep_u64(i, 0x1EF0) | _pdep_u64(i >> 8, 0x1EF000000000) | 0x100;
        state[i] = check_state(line);
    }
#if Debug
    cout << "Total : " << count << " nodes." << endl;
    for(int i = 0; i < 65536; i++)
    {
        if((i >> 8) & i)continue;
        for(int k = 0; k < 4; k++)
        {
            int tmp = (i & (0x101 << k)) >> k;
            if(tmp == 1)cout << "1";
            else if(tmp == 0x100)cout << "0";
            else cout << ".";
        }
        cout << "x";
        for(int k = 4; k < 8; k++)
        {
            int tmp = (i & (0x101 << k)) >> k;
            if(tmp == 1)cout << "1";
            else if(tmp == 0x100)cout << "0";
            else cout << ".";
        }
        cout << " : ";
        switch(state[i])
        {
        case 8:
            cout << "L5";
            break;
        case 4:
            cout << "L4";
            break;
        case 2:
            cout << "D4";
            break;
        case 1:
            cout << "L3";
            break;
        case 0:
            break;
        default:
            cout << "WTF";
            break;
        }
        cout << endl;
    }
#endif // Debug
    //c++ code
    cout << "static constexpr uint8_t state[65536] =" << endl;
    cout << "{" << endl;
    for(int i = 0, k = 0; i < 65536; i++, k++)
    {
        if(k == 0)cout << "    ";
        cout << (int)state[i] << ",";
        if(k == 63){cout << endl;k = -1;}
    }
    cout << "};" << endl;
    return 0;
}
