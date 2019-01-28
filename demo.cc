#include <iostream>
#include <fstream>
#include <sstream>
#include "bitboard.h"
#include "state.h"
#include "Tboard.h"
#include "Cboard.h"
using namespace std;
void init()
{
    ifstream fin;
    fin.open("data/Tboard.dat", ios::binary | ios::in);
    fin.read((char *)Tboard_h, sizeof(Tboard_h));
    fin.read((char *)Tboard_b, sizeof(Tboard_b));
    fin.read((char *)Tboard_v, sizeof(Tboard_v));
    fin.read((char *)Tboard_s, sizeof(Tboard_s));
    fin.close();
    fin.open("data/Cboard.dat", ios::binary | ios::in);
    fin.read((char *)Cboard_h, sizeof(Cboard_h));
    fin.read((char *)Cboard_b, sizeof(Cboard_b));
    fin.read((char *)Cboard_v, sizeof(Cboard_v));
    fin.read((char *)Cboard_s, sizeof(Cboard_s));
    fin.close();
}
BitBoard endmy, endopp, endRzone;
int TSS(BitBoard my, BitBoard opp, BitBoard Tzone, int depth)
{
    if(depth <= 0)return 0;
    BitBoard cTzone = Tzone & my & opp;
    int check = 0;
    while(cTzone && !check)
    {
        uint64_t pos = cTzone.ls1b();
        uint64_t feat = feature(my, opp, pos);
        uint64_t s_h = state[_pext_u64(feat, pext_h)];
        uint64_t s_b = state[_pext_u64(feat, pext_b)];
        uint64_t s_v = state[_pext_u64(feat, pext_v)];
        uint64_t s_s = state[_pext_u64(feat, pext_s)];
        my.append(pos);
        if((s_h | s_b | s_v | s_s) & 0xC0000) //terminal condition
        {
            if(s_h & 0x40000)
            {
                opp = opp & Cboard_h[(pos << 5) | (s_h & 0x1F)];
            }
            else if (s_b & 0x40000)
            {
                opp = opp & Cboard_b[(pos << 5) | (s_b & 0x1F)];
            }
            else if (s_v & 0x40000)
            {
                opp = opp & Cboard_v[(pos << 5) | (s_v & 0x1F)];
            }
            else if (s_s & 0x40000)
            {
                opp = opp & Cboard_s[(pos << 5) | (s_s & 0x1F)];
            }
            endmy = my;
            endopp = opp;
            return depth;
        }
        if(s_h & 0x30000)
        {
            check = TSS(my,
                        opp & Cboard_h[(pos << 5) | (s_h & 0x1F)],
                        Tzone
                        |Tboard_h[(s_h & 0x3F00) | pos]
                        |Tboard_b[(s_b & 0x3F00) | pos]
                        |Tboard_v[(s_v & 0x3F00) | pos]
                        |Tboard_s[(s_s & 0x3F00) | pos],
                        depth-1);
        }
        else if(s_b & 0x30000)
        {
            check = TSS(my,
                        opp & Cboard_b[(pos << 5) | (s_b & 0x1F)],
                        Tzone
                        |Tboard_h[(s_h & 0x3F00) | pos]
                        |Tboard_b[(s_b & 0x3F00) | pos]
                        |Tboard_v[(s_v & 0x3F00) | pos]
                        |Tboard_s[(s_s & 0x3F00) | pos],
                        depth-1);
        }
        else if(s_v & 0x30000)
        {
            check = TSS(my,
                        opp & Cboard_v[(pos << 5) | (s_v & 0x1F)],
                        Tzone
                        |Tboard_h[(s_h & 0x3F00) | pos]
                        |Tboard_b[(s_b & 0x3F00) | pos]
                        |Tboard_v[(s_v & 0x3F00) | pos]
                        |Tboard_s[(s_s & 0x3F00) | pos],
                        depth-1);
        }
        else if(s_s & 0x30000)
        {
            check = TSS(my,
                        opp & Cboard_s[(pos << 5) | (s_s & 0x1F)],
                        Tzone
                        |Tboard_h[(s_h & 0x3F00) | pos]
                        |Tboard_b[(s_b & 0x3F00) | pos]
                        |Tboard_v[(s_v & 0x3F00) | pos]
                        |Tboard_s[(s_s & 0x3F00) | pos],
                        depth-1);
        }
        else
        {
            Tzone.append(pos);
        }
        my.append(pos);
    }
    return check;
}
bool canThreat(uint64_t feat)
{
    uint64_t s_h = state[_pext_u64(feat, pext_h)];
    uint64_t s_b = state[_pext_u64(feat, pext_b)];
    uint64_t s_v = state[_pext_u64(feat, pext_v)];
    uint64_t s_s = state[_pext_u64(feat, pext_s)];
    if((s_h | s_b | s_v | s_s) & 0xE0000) //L5, L4, D4
    {
        return true;
    }
    return false;
}
void NM(BitBoard my, BitBoard opp, BitBoard Tzone, int depth)
{
    int check = 0;
    for(int i = 1; i <= depth/2 && !check; i++)
    {
        check = TSS(my, opp, Tzone, i);
    }
    endRzone.initZero();
    if(check)
    {
        BitBoard empty = endmy & endopp;
        while(empty)
        {
            uint64_t pos = empty.ls1b();
            uint64_t feat = feature(endopp, endmy, pos);
            if(canThreat(feat))
            {
                endRzone.append(pos);
            }
        }
        endRzone = endRzone | (my^endmy) | (opp^endopp);
    }
}
int ABSearch(BitBoard my, BitBoard opp, BitBoard myTzone, BitBoard oppTzone, BitBoard moves,BitBoard Rzone, int alpha, int beta, int depth, int score)
{
    if(depth <= 0)return score;
    BitBoard mov = Rzone ? Rzone : (moves & my & opp);
    while(mov)
    {
        uint64_t pos = mov.ls1b();
        uint64_t feat = feature(my, opp, pos);
        uint64_t s_h = state[_pext_u64(feat, pext_h)];
        uint64_t s_b = state[_pext_u64(feat, pext_b)];
        uint64_t s_v = state[_pext_u64(feat, pext_v)];
        uint64_t s_s = state[_pext_u64(feat, pext_s)];
        BitBoard newTzone = myTzone
                            |Tboard_h[(s_h & 0x3F00) | pos]
                            |Tboard_b[(s_b & 0x3F00) | pos]
                            |Tboard_v[(s_v & 0x3F00) | pos]
                            |Tboard_s[(s_s & 0x3F00) | pos];
        if((s_h | s_b | s_v | s_s) & 0xC0000) //L5, L4
        {
            return 10000;
        }
        my.append(pos);
        NM(my, opp, newTzone, depth);
        int value = -ABSearch(opp, my, oppTzone, newTzone, moves.mind(pos), endRzone, -beta, -alpha, depth-1, -score);
        if(value >= 10000)return value;
        if(value >= beta)return value;
        if(value > alpha)alpha = value;
        my.append(pos);
    }
    return alpha;
}
uint64_t ABCaller(BitBoard my, BitBoard opp, BitBoard myTzone, BitBoard oppTzone, BitBoard moves, int alpha, int beta, int depth, int score)
{
    if(depth <= 0)return score;
    BitBoard mov = moves & my & opp;
    uint64_t bestmove = 0;
    while(mov)
    {
        uint64_t pos = mov.ls1b();
        uint64_t feat = feature(my, opp, pos);
        uint64_t s_h = state[_pext_u64(feat, pext_h)];
        uint64_t s_b = state[_pext_u64(feat, pext_b)];
        uint64_t s_v = state[_pext_u64(feat, pext_v)];
        uint64_t s_s = state[_pext_u64(feat, pext_s)];
        BitBoard newTzone = myTzone
                            |Tboard_h[(s_h & 0x3F00) | pos]
                            |Tboard_b[(s_b & 0x3F00) | pos]
                            |Tboard_v[(s_v & 0x3F00) | pos]
                            |Tboard_s[(s_s & 0x3F00) | pos];
        if((s_h | s_b | s_v | s_s) & 0x80000) //L5
        {
            return pos;
        }
        my.append(pos);
        NM(my, opp, newTzone, depth);
        int value = -ABSearch(opp, my, oppTzone, newTzone, moves.mind(pos), endRzone, -beta, -alpha, depth-1, -score);
        if(value > alpha)
        {
            alpha = value;
            bestmove = pos;
        }
        my.append(pos);
    }
    return bestmove;
}
void Output(uint64_t pos)
{
    int i = pos / 16;
    int j = pos % 16;
    cout << char('A'+j-1);
    cout << 15-i;
    cout << std::endl;
}
uint64_t Input(char c, int n)
{
    return (15-n)*16 + c - 'A' + 1;
}
struct Game
{
    BitBoard black, white, moves, blackTzone, whiteTzone;
    BitBoard black_record[256], white_record[256], moves_record[256], blackTzone_record[256], whiteTzone_record[256];
    int size = 0, score = 0, score_record[256];
    void Start();
    void Move(uint64_t pos);
    void Undo();
    void Play(int depth);
};
void Game::Start()
{
    black.init();
    white.init();
    moves.initZero();
    blackTzone.initZero();
    whiteTzone.initZero();
    size = 0;
    score = 0;
}
void Game::Move(uint64_t pos)
{
    black_record[size] = black;
    white_record[size] = white;
    moves_record[size] = moves;
    score_record[size] = score;
    blackTzone_record[size] = blackTzone;
    whiteTzone_record[size] = whiteTzone;
    uint64_t feat;
    if(size%2 == 0)
    {
        feat = feature(black, white, pos);
        black.append(pos);
        uint64_t s_h = state[_pext_u64(feat, pext_h)];
        uint64_t s_b = state[_pext_u64(feat, pext_b)];
        uint64_t s_v = state[_pext_u64(feat, pext_v)];
        uint64_t s_s = state[_pext_u64(feat, pext_s)];
        blackTzone = blackTzone
                    |Tboard_h[(s_h & 0x3F00) | pos]
                    |Tboard_b[(s_b & 0x3F00) | pos]
                    |Tboard_v[(s_v & 0x3F00) | pos]
                    |Tboard_s[(s_s & 0x3F00) | pos];
    }
    else
    {
        feat = feature(white, black, pos);
        white.append(pos);
        uint64_t s_h = state[_pext_u64(feat, pext_h)];
        uint64_t s_b = state[_pext_u64(feat, pext_b)];
        uint64_t s_v = state[_pext_u64(feat, pext_v)];
        uint64_t s_s = state[_pext_u64(feat, pext_s)];
        whiteTzone = whiteTzone
                    |Tboard_h[(s_h & 0x3F00) | pos]
                    |Tboard_b[(s_b & 0x3F00) | pos]
                    |Tboard_v[(s_v & 0x3F00) | pos]
                    |Tboard_s[(s_s & 0x3F00) | pos];
    }
    moves = moves.mind(pos);
    score += 0;/**by feat**/
    score = -score;
    size += 1;
}
void Game::Undo()
{
    if(size > 0)
    {
        size -= 1;
        black = black_record[size];
        white = white_record[size];
        moves = moves_record[size];
        score = score_record[size];
        blackTzone = blackTzone_record[size];
        whiteTzone = whiteTzone_record[size];
    }
}
void Game::Play(int depth)
{
    uint64_t pos = 0;
    if(size%2 == 0)
    {
        if (size == 0)
        {
            moves.append(114);
        }
        for(int i = 1; i <= depth; i++)
        {
            pos = ABCaller(black, white, blackTzone, whiteTzone, moves, -30000, 30000, i, score);
        }
    }
    else
    {
        pos = 0;
        if(size == 1)
        {
            BitBoard tmp;
            tmp.append(114);
            if(!(black ^ tmp))
            {
                pos = 117;
            }
            tmp.append(114);
            tmp.append(24);
            if(!(black ^ tmp))
            {
                pos = 72;
            }
            tmp.append(24);
            tmp.append(126);
            if(!(black ^ tmp))
            {
                pos = 123;
            }
            tmp.append(126);
            tmp.append(216);
            if(!(black ^ tmp))
            {
                pos = 168;
            }
            tmp.append(216);
        }
        if(pos == 0)
        {
            for(int i = 1; i <= depth; i++)
            {
                pos = ABCaller(white, black, whiteTzone, blackTzone, moves, -30000, 30000, i, score);
            }
        }
    }
    Move(pos);
    Output(pos);
}
int main()
{
    init();
    Game game;
    int depth;
    string op;
    while(cin >> op)
    {
        if(op == "TERMINATE")
        {
            break;
        }
        else if(op == "START")
        {
            game.Start();
            cout << "ROGER" << endl;
        }
        else if(op == "PLAY")
        {
            cin >> depth;
            game.Play(depth);
        }
        else if(op == "UNDO")
        {
            game.Undo();
            cout << "ROGER" << endl;
        }
        else if('A' <= (op[0]&0xDF) && (op[0]&0xDF) <= 'O')
        {
            stringstream ss;
            op[0] &= 0xDF;
            ss << op;
            char c;
            int n;
            if(ss >> c >> n)
            {
                uint64_t pos = Input(c, n);
                game.Move(pos);
                cout << "ROGER" << endl;
            }
            else
            {
                cout << "WRONG" << endl;
            }
        }
        else
        {
            cout << "WRONG" << endl;
        }
    }
    return 0;
}
