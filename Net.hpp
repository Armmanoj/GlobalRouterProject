#ifndef MY_INCLUDES_H
#include "main.hpp"
#endif
class GAMER;
class Grid_Graph;

class Net {
    public:
        std::vector<Point> pins;
        std::vector<Pair> route;
        std::vector<Point> metalzero;
        std::string name;
        Point cornerl;
        Point cornerh;
        Net(Grid_Graph& G, std::vector<Point>& pin_list, std::string& Name, GAMER& GAMER);
        //void pattern_route(Grid_Graph G);
        void maze_route(Grid_Graph& G, GAMER& g, int alternations, bool first_time);
        float traverse_line(Grid_Graph& G, Pair pair,int increment);
        float traverse_route(Grid_Graph& G, int increment);
        void back_trace(Grid_Graph& G, GAMER& g, Point dst);
        void bounding_box(float k, int box_min_side,int M, int N);
};