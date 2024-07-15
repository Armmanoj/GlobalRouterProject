
#ifndef MY_INCLUDES_H
#ifndef MY_INCLUDES_H
#include "main.hpp"
#endif
#endif
class Grid_Graph;

class GAMER{
    public:
        std::vector<std::vector<float>> Sdist; // for metal1
        std::vector<std::vector<char>> Sdir;
        GAMER(int M, int N, int L);
        void Init_GAMER(Grid_Graph& G,Point src, Point cornerl, Point cornerh, int first);
        void algo(Grid_Graph& G,Point cornerl, Point cornerh, int alternations, int lay);
        void printpath(int L,int M,int N);
};