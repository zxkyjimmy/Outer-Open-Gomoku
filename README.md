# Outer-Open-Gomoku
This is a demo of OOGiveMeFive without sliding window. OOGiveMeFive is a program for the game Outer-Open Gomoku. It used the bitboard with some efficient instructions to get the situation inside the **pozidriv-shaped mask (M mask)** at each action.
The detail is in our paper "[An algorithmic design and implementation of outer-open gomoku](https://ieeexplore.ieee.org/document/8075180)"
## Citing OOGiveMeFive
If our research can help you, please consider citing the paper as follows:
```latex
@inproceedings{chen2017oog,
  author = {Chih-Hung Chen and Shun-Shii Lin and Yen-Chi Chen},
  booktitle = {2017 2nd International Conference on Computer and Communication Systems (ICCCS)},
  title = {An algorithmic design and implementation of outer-open gomoku},
  pages = {26-30},
  doi = {10.1109/CCOMS.2017.8075180},
  year = {2017},
  month = {7},
  address = {Kracow, Poland}
}
```
If you use OOGiveMeFive, please consider citing the library as follows:
```latex
@mise{OOGiveMeFive
  author = {Yen-Chi Chen and Chih-Hung Chen and Shun-Shii Lin},
  title = {OOGiveMeFive},
  year = {2017},
  journal = {GitHub repository},
  url = {https://github.com/zxkyjimmy/Outer-Open-Gomoku}
}
```
## Dependencies
To build OOGiveMeFive, you only need:
- GCC (g++) **7.3**
- makefile

Please check whether there are the following instructions or instruction sets in your CPU or not. If not, the only thing you can do is to buy a new one.
- popcnt
- lzcnt
- bmi
- bmi2
## Building
`cd` to the project root and run `make` to build.
## Appendices
In order to speed up the performance, this program has many parameters that are prepared in advance. In the **appendices** folder, I collect the code used to generate parameters.
### `state[65536]`
- version 1: traditional bitboard
- version 2: inverse bitboard
- version 3: Add threat zone state
- version 4: Consider defense state

We consider an **M mask** of length 4 from the center.
For each **M mask**, there are 4 lines - horizontal, vertical, slash and backslash.
Since the center must be the action, we only need to collect 32 positions except for the center.
In addition, we have two bitboards (your's own and opponent's).
A total of 64 bits are required. All of them are collected in the `feature` variable.
When we inquire about the status of the action, we divide it into 4 lines.
Therefore, for each line, there are 2^16 = 65536 possibilities.

Version 1 is for traditional bitboard (no inverse).
It's used to distinguish the state (L5, L4, D4, L3).

Version 2 is for inverse bitboard.
With traditional bitboard, when we use the instruction `pext` (Parallel bits extract) to extract the feature, we can't distinguish between **empty** and **out of wall**, so we need extra processing.
However, they will be different if we use inverse bitboard.

Version 3 and 4 weren't described in our paper. It's just for my personal experimentation.
