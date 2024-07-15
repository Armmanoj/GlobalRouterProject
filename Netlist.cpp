#include "Net.hpp"
#include "Netlist.hpp"
#ifndef MY_INCLUDES_H
#include "main.hpp"
#endif
#include "Grid_Graph.hpp"
#include "Gamer.hpp"

int isPointNotInPins(const std::vector<Point>& pins, int x, int y, int z){
    Point target = {x, y, z};

    // Check if the target point exists in the pins vector
    for (const auto& pin : pins) {
        if (pin == target) {
            return 0; // Point is present
        }
    }
    return 1; // Point is not present
}

Netlist::Netlist(Grid_Graph& G, std::string& netfile,GAMER& gamer){
    std::ifstream file(netfile);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << netfile << std::endl;
        return;
    }
    std::string line;
    std:: string name;
    int x, y, l;
    int count=0;
    std::vector<Point> pins;
    while (std::getline(file, line)){
        pins.clear();
        name = line;
        std::getline(file, line);
        std::getline(file, line);
         while (line[0]=='['){
            sscanf(line.c_str(), "[(%d, %d, %d)", &l, &x, &y); // Parse the first access point for simplicity
            //if (isPointNotInPins(pins,x,y,l-1));
           // {
                pins.push_back(Point(x,y,l-1)); // actual routing starts on metal1 only
            //}
            std::getline(file, line);
         }
        nets.emplace_back(G, pins, name, gamer); // add the new net to netlist 
        if (count%2000==1){
            std::cout << "Net number " << count << " is made\n";
        }
        count++;
    }
}

void Netlist::mazer(Grid_Graph& G, GAMER& GAMER){
    for (int i = 0; i<nets.size(); i++){
        nets[i].maze_route(G, GAMER,ALTERNATIONS,false); // ALTERNATIONS is max number of alternations in GAMER
    }
}

void Netlist::PR_output(std::string& outfile){
    std::ofstream file(outfile);
    Point p,q;
    for (int i = 0; i<nets.size(); i++){
        file << nets[i].name << "\n(\n";
        for (int j=0; j<nets[i].route.size(); j++){
            Pair pair = nets[i].route[j];
            p = pair.p; q = pair.q;
            if ((p.x==q.x)&&(p.y!=q.y)){
                if (p.y>q.y){
                    p = pair.q;
                    q = pair.p;
                }
            }
            else if ((p.y==q.y)&&(p.x!=q.x)){
                if (p.x>q.x){
                    p = pair.q;
                    q = pair.p;
                }
            }
            if (p!=q){ // This happens if we go from x,y,0 to x,y,1 only, it is read as x,y,1 to x,y,1
                if (p.l>q.l){
                        p = pair.q;
                        q = pair.p;
                    }
                file << p.x << " " << p.y << " " << p.l+1 << " " << q.x << " " << q.y << " " << q.l+1 << std::endl;
            }
            // adding +1 to all layer's as metal1 is layer 0 here, assumption is x y -1 x y l is stored in route an endpoint vertical line
        }
        for (int j=0; j<nets[i].metalzero.size(); j++){
            p = nets[i].metalzero[j];
            file << p.x << " " << p.y << " " << 0 << " " <<  p.x << " " << p.y << " " << 1 << std::endl; 
        }
        file << ")\n";
    }
}