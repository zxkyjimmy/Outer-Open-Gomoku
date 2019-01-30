# Outer-Open-Gomoku
This is a demo of OOGiveMeFive without sliding window. OOGiveMeFive is a program for the game Outer-Open Gomoku. It used the bitboard with some efficient instructions to get the situation inside the **pozidriv-shaped mask** at each action(move).
The detail is in our paper "[An algorithmic design and implementation of outer-open gomoku](https://ieeexplore.ieee.org/document/8075180)"
## Citing OOGiveMeFive
If you use OOGiveMeFive, please consider citing the library as follows:
```latex
@mise{OOGiveMeFive
  author = {Yen-Chi Chen and Chih-Hung Chen and Shun-Shii Lin},
  title = {OOGiveMeFive},
  year = {2017},
  journal = {GitHub repository},
  howpublished = {\url{https://github.com/zxkyjimmy/Outer-Open-Gomoku}}
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
