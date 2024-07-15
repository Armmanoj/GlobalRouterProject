#include "Net.hpp"
#include "Netlist.hpp"
#ifndef MY_INCLUDES_H
#include "main.hpp"
#endif
#include "Grid_Graph.hpp"
#include "Gamer.hpp"
int num_thread = 1;

/*
Improvements possible-
1.  Replace read data from file and store data to file with easier to understand names, or/and incorporate them into netlist and grid_graph class
2.  Include all the test fnctions into a seperate cpp file which can be included into main as #include "test_displays.cpp"
3.  Simplify the constructor of netlist and Grid_Graph, to constuct directly from file being read, as well as have a destructor for both, which will store the results appropritely
4.  Simplify the construction of data structure that stores the routes, making sure of how memory is handled in it
5.  Reduce the importance of "Batch" class to near zero, do al routing in some nets class itself
*/

int main(int argc, char *argv[]) {
    /*
--- variables initialized- 
    M rows
    N cols
    C capacity
    V cost of bending
    x1,x2,y1,y2
    Gx,Gy grid graph edge weights
    net_x stores x coordinates of corners in each path
    net_y stores y coordinates of edges in each path
--- functions called-
    ReadDataFromFile
    Router
    */
    if (argc != 6) {
        std::cerr << "Correct format is " << argv[0] << " <.net> <.cap> <.PR_output>  <Number of Iterations for Maze Route> <Number of Threads for Parallelising>\n";
        return 1;
    }
     
    std::string netfilename(argv[1]);
    std::string capfilename(argv[2]);
    std::string outfilename(argv[3]);
    int MAZE_ROUTE_ITER = atoi(argv[4]);
    num_thread = atoi(argv[5]);
    
    Grid_Graph G(capfilename);
    std::cout << "Grid graph successfully made" << std::endl;
    GAMER gamer(G.M,G.N,G.L);
    std::cout << "GAMER initialized " << std::endl;
    Netlist Netlist(G,netfilename,gamer);
    std::cout << "Netlist successfully made" << std::endl;
    double T__ = omp_get_wtime();
    Netlist.mazer(G,gamer);
    double T___ = omp_get_wtime();
    std::cerr << "Maze routing takes  " << T___ -T__  << " seconds" << std::endl; 
    // Store the result
    Netlist.PR_output(outfilename);
    std:: cerr << "Output succesfully stored to file " << std::endl;
    return 0;
}
