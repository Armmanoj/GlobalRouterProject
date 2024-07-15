#define ALTERNATIONS 8
#ifndef MY_INCLUDES_H
#include "main.hpp"
#endif
class GAMER;
class Grid_Graph;
class Net;

class Netlist {
public:
    std::vector<Net> nets;      // Vector of nets
    // Constructor
    Netlist(Grid_Graph& G,std::string& netfile, GAMER& gamer);
    // Function to perform maze routing
    void mazer(Grid_Graph& G, GAMER& GAMER);
    void PR_output(std::string& outfile);
};



