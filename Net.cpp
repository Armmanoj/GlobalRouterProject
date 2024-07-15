#define BOX_SCALING 1.75
#define BOX_MIN_SIDE 8
#include "Net.hpp"
#include "Netlist.hpp"
#ifndef MY_INCLUDES_H
#include "main.hpp"
#endif
#include "Grid_Graph.hpp"
#include "Gamer.hpp"

// Defining the constructor
Net::Net(Grid_Graph& G, std::vector<Point>& pin_list, std::string& Name, GAMER& GAMER){
    for (int i=0; i<Name.size(); i++){
        name.push_back(Name[i]);
    }
    route.reserve(4*pin_list.size());
    pins.reserve(pin_list.size());
    for (int k = 0; k<pin_list.size(); k++){
        if (pin_list[k].l == -1){
            metalzero.push_back(pin_list[k]);
            pin_list[k].l = 0;
        }
        pins.push_back(pin_list[k]);
    } 
    bounding_box(BOX_SCALING,BOX_MIN_SIDE, G.N,G.M); // calculate the net's bounding box
    // set the vectors size after setting its initial memory usage
    //Tree t = make_tree(G);
    //for (int i=0; i<2*t.deg-2; i++) {
    //  pattern_route(G,Point(t.branch[i].x, t.branch[i].y), Point(t.branch[t.branch[i].n].x,t.branch[t.branch[i].n].y));
    //}
    //std::cout << "Issue is in mazer" << std::endl;
    //std::cout << "pins contains:\n";
    //for (const auto& point : pins) {
        //std::cout << point.x << " " << point.y << " " << point.l << "\n";
    //}
    //std::cout << std::endl;
    ////std::cout << "metalzero contains:\n";
    //for (const auto& point : metalzero) {
        //std::cout << point.x << " " << point.y << " " << point.l << "\n";
    //}
    //std::cout << std::endl;
    maze_route(G, GAMER, 3, true);
}

// Code to choose the best L shaped route for the net
/*void Net::pattern_route(Grid_Graph G, Point Src, Point Dst){
    if (Src.l%2==Dst.l%2){

    }
    else{

    }
    if (!((Src.x == Dst.x)||(Src.y == Dst.y))){
        route[0] = Point(Src.x,Dst.y,0);
        float weight1 = traverse_route(G,0);
        route[0] = Point(Dst.x,Src.y,0);
        float weight2 = traverse_route(G,0);
        route[0] = (weight2>weight1) ? Point(Dst.x,s=Src.y,0) : Point(Src.x,Dst.y,0);
    }
    traverse_route(G,1);  
}*/

void Net::maze_route(Grid_Graph& G, GAMER& g, int alternations, bool first_time){
    if (!first_time){
        traverse_route(G,-1); // rip up
    }
    route.clear(); // rip up
    // initialize the shortest distances
    //double T = omp_get_wtime();
    //std::cout<< "Going to initialize the gamer " << std::endl;
    g.Init_GAMER(G,pins[0],cornerl,cornerh,1);
    //double T_ = omp_get_wtime();
    //std::cout<<"Gamer INit time " << T_-T <<std::endl;
    //std::cout << "Initialized GAMER" << std::endl;
    for (int i=1; i<pins.size(); i++){
        // running the relaxations
        //T = omp_get_wtime();
        g.algo(G,cornerl,cornerh,alternations,G.L);
        //T_ = omp_get_wtime();
        //std::cout<<"Algo time " << T_-T <<std::endl;
        //T = omp_get_wtime();
        // update grid_graph and set distances along path to zero
        //std::cout << "Backtracing next " << std::endl;
        back_trace(G,g,pins[i]); // labels routes as new source, appends to routes
        //T_ = omp_get_wtime();
        //std::cout<<"bt time " << T_-T <<std::endl;
        // set to infinity all distances not on the old routes
        //std::cout << "Backtracing done " << std::endl;
        //double T = omp_get_wtime();
        //std::cout<< "Going to initialize the gamer again " << std::endl;
        g.Init_GAMER(G,pins[0],cornerl,cornerh,0);
        //double T_ = omp_get_wtime();
        //std::cout<<"Gamer grid INit time " << T_-T <<std::endl;
    }
    traverse_route(G,1); // updates grid graph
}

void Net::back_trace(Grid_Graph& G, GAMER& g, Point dst){
    //backtracing
    Point here = dst;
    Point was_here = dst;
    char dir,diro;
    // initializing diro
    if (here.l%2){diro = g.Sdir[here.l][G.N*here.x+here.y];}
    // -1 % 2 == -1 , not 1, this implementation quirk means we cannot check if here.l%2==1 
    else{diro = g.Sdir[here.l][G.M*here.y+here.x];}   
    bool flag = true;
    //std::cout << "started backtracing " << std::endl;
    int counter = 0;
    while (flag){
        //std::cout << "in while loop " << std::endl;
        // step 1 : decide which direction to move
        if (here.l%2){dir = g.Sdir[here.l][G.N*here.x+here.y];}
        else{dir = g.Sdir[here.l][G.M*here.y+here.x];}
        // step 2 : update Sdist and Sdir "here"
        if (here.l%2){
            g.Sdist[here.l][G.N*here.x+here.y] = 0;
            g.Sdir[here.l][G.N*here.x+here.y] = 'x';
        }
        else{
            g.Sdist[here.l][G.M*here.y+here.x] = 0;
            g.Sdir[here.l][G.M*here.y+here.x] = 'x';
        }
        //std::cout << here.x << " " << here.y << " " << here.l << std::endl;
        // step 3 : if dir!=diro, append to route, update diro, and update route list
        if (dir != diro){
            route.emplace_back(here,was_here); // push_back has amortized O(1) time complexity
            was_here = here;
        }
        diro = dir;
        
        // step 4 : move "here" 1 step
        switch(dir){
            case 'd':
                here.l--; break;
            case 'u':
                here.l++; break;
            case 'l':
                here.x--; break;
            case 'r':
                here.x++; break;
            case 'n':
                here.y++; break;
            case 's':
                here.y--; break;
            default: break;
        }   
        // step 5: if "here" has an Sdist of 0, store the last line and break
        if (here.l%2){
            if (!g.Sdist[here.l][G.N*here.x+here.y]){
                //std::cout << here.x << " " << here.y << " " << here.l << std::endl;
                flag = false;
                route.emplace_back(here,was_here); // push_back has amortized O(1) time complexity
                was_here = here;
            }
        }
        else{
            if (!g.Sdist[here.l][G.M*here.y+here.x]){
                //std::cout << here.x << " " << here.y << " " << here.l << std::endl;
                flag = false;
                route.emplace_back(here,was_here); // push_back has amortized O(1) time complexity
                was_here = here;
            }
        }
    }
}

float Net::traverse_line(Grid_Graph& G, Pair pair,int increment)
{
    /*
    if increment is +1, we are incrementing grid_graphs edge demand (routing the net) on traversing through that edge,
    for increment=-1, we are "ripping" the net by decrementing Grid_Graph demand, increment = 0 for just calculating cost of a route
    The assumption is that atleast 2 of x,y,l are same for Src and Dest
    */
    Point Src = pair.p;
    Point Dest = pair.q;
    float sum = 0; 
    int l = Src.l;
    int x = Src.x;
    int y = Src.y;
    if (Src.x == Dest.x) {
        // north-south line
        for (int y = std::min(Src.y,Dest.y)+1; y < std::max(Dest.y,Src.y); ++y) {
            G.G[l][Src.x*(G.N+1)+y] += increment;
            sum += G.weight(l,x,y);
        }
    }
    else if (Src.y == Dest.y){
        // east-west line
        for (int x = std::min(Src.x,Dest.x) + 1; x < std::max(Dest.x,Src.x); ++x) {
            G.G[l][Src.y*(G.M+1)+x] += increment;
            sum += G.weight(l,x,y);
        }
    }
    else if (Src.l==Dest.l){
        sum += G.v*std::abs(Src.l-Dest.l);
    }
    else{
        //std::cout << "Error, not a line to traverse" << std::endl;
        exit(1);
    }
    return sum;
}

float Net::traverse_route(Grid_Graph& G, int increment)
{
    /*
    if increment is +1, we are incrementing grid_graphs edge demand (routing the net) on traversing through that edge,
    for increment=-1, we are "ripping" the net by decrementing Grid_Graph demand
    */
    float sum = 0;
    for (const auto& line : route){
        sum += traverse_line(G, line,increment);
    }
    return sum;
}

/*
GPT prompt- Let pins be a vector of "Point" elements, where point has an x,y. Define the bounding box of the pins as a box whose x
side length is k times the max distance between x values of points in pins and whose y side length is k times the max distance 
between y values of pins, centered at x = midpoint of the 2 furthest apart points along x direction and y= midpoint of the 2 
furthest apart points in y direction. Write code that calculates the lower and upper corners of bounding box
*/
void Net::bounding_box(float k, int box_min_side,int M, int N){

    double minX = std::numeric_limits<double>::max();
    double maxX = 0;
    double minY = std::numeric_limits<double>::max();
    double maxY = 0;

    // Find the min and max values for x and y
    for (const auto& p : pins) {
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }

    // Calculate the midpoints
    double midX = (minX + maxX) / 2.0;
    double midY = (minY + maxY) / 2.0;

    // Calculate the side lengths of the bounding box
    double sideLengthX = std::max(k * (maxX - minX),(double)box_min_side);
    double sideLengthY = std::max(k * (maxY - minY),(double)box_min_side);

    // Calculate the lower and upper corners
    cornerl = Point(std::max((int)(midX - sideLengthX / 2.0),0),std::max((int)(midY - sideLengthY / 2.0),0),0);
    cornerh = Point(std::min((int)(midX + sideLengthX / 2.0),M-1),std::min((int)(midY + sideLengthY / 2.0),N-1),0);
}