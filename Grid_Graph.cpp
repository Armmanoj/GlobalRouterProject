#include "Net.hpp"
#include "Netlist.hpp"
#ifndef MY_INCLUDES_H
#include "main.hpp"
#endif
#include "Grid_Graph.hpp"
#include "Gamer.hpp"

// Constructor
Grid_Graph::Grid_Graph(std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    // storing the short, wire and via costs
    std::string line;
    infile >> L >> M >> N;
    L--;
    infile >> wire_cost >> v; // Nlayers, xSize ySize
    float shortcosts;
    infile >> shortcosts;
    for (int k = 0; k<L; k++){
        infile >> shortcosts;
        short_cost.push_back(shortcosts);
    }
    
    // Creating the vector of demand, G
    for (int k = 0; k < L; ++k) {
        if (k % 2 == 0) {
            G.push_back(std::vector<int>((M + 1) * N, 0));
        }
        else {
            G.push_back(std::vector<int>((N + 1) * M, 0));
        }
    }

    // creating the vector of capacity C
    float cap_float;
    for (int k = 0; k < L; ++k) {
        // read the rest of the first line
        while (std::getline(infile, line)) {
            if (line.find("metal") != std::string::npos && line.find("metal1")== std::string::npos) {
                // Stop processing when "metal" is found
                break;
            }
        }
        if (k % 2 == 0) {
            C.push_back(std::vector<int>(((M + 1) * N), 0));
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j <= M; ++j) {
                    // j=0 has C=0 ie, edge of chip but this is not given in .cap file format
                    infile >> cap_float;
                    C[k][i*(M+1)+j] = (int)cap_float;
                }
                C[k][i*(M+1)+M]=0;
            }
        }
        else {
            C.push_back(std::vector<int>((N + 1) * M, 0));
            for (int j = 0; j < M; ++j) {
                // j = 0 has C=0, but not there in file format, also, file format is the transpose of this format
                for (int i = 0; i < N; ++i){
                    infile >> cap_float;
                    C[k][j*(N+1)+i]= (int)cap_float;
                }
                C[k][j*(N+1)+N]=0;
            }
        }
    } 
    std::cout << "size is " <<L<<" "<<M<<" "<<N<<std::endl;
}

// calculate number of electrical shorts in the chip
int Grid_Graph::overflows() {
    int count = 0;
    for (int k = 0; k < L; ++k) {
        if (k % 2 == 0) {
            for (int i = 0; i < (N + 1) * M; i++) {
                if (G[k][i] > C[k][i]) {
                    count+=G[k][i]-C[k][i];
                }
            }
        }
        else {
            for (int i = 0; i < (M + 1) * N; i++) {
                if (G[k][i] > C[k][i]) {
                    count+=G[k][i]-C[k][i];
                }
            }
        }
    }
    return count;
}

float Grid_Graph::weight(int l, int x, int y){
    if (l%2==0){
        // the return values of this function can be memoised for further speedup
        if (!C[l][x*(M+1)+y]){
            return short_cost[l];
        }
        return wire_cost*3000-short_cost[l] + 2*short_cost[l]/(1+std::exp(-float(G[l][x*(M+1)+y])/float(C[l][x*(M+1)+y])));
    }
    else{
        if (!C[l][x*(M+1)+y]){
            return short_cost[l];
        }
        return wire_cost*3000-short_cost[l] + 2*short_cost[l]/(1+std::exp(-float(G[l][y*(N+1)+x])/float(C[l][y*(N+1)+x])));
    }
}