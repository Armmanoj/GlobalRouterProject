#define GRID_H
#ifndef MY_INCLUDES_H
#include "main.hpp"
#endif

class Grid_Graph {
public:
    std::vector<std::vector<int>> G;
    std::vector<std::vector<int>> C;
    float wire_cost,v; // wire and via cost
    std::vector<float> short_cost;
    int L, M, N; // Nlayers xSize ySize
    // Constructor
    Grid_Graph(std::string& filename);
    // Function to calculate number of overflows
    int overflows();
    float weight(int l, int i, int j);
};