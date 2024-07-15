
#define MY_INCLUDES_H

#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <limits>
#include <ctime>
#include <algorithm>
#include <random>
#include <omp.h>

#define ROUTE_MEM_ESTIMATE 10
//#define BOX_MIN_DIM 9 // determines the minimum width or length of bounding box, can be 32 say
//#define MAZE_ROUTE_ITER 1
//#define NUM_THREADS 4
// MY_INCLUDES_H

typedef struct {
  int x, y ;   	// starting point of the branch
  int n ;        	// index of neighbor
} Branch ;

typedef struct {	/* in proper packing order */
    Branch *branch ;   	// array of tree branches
    int length;		// total wirelength
    int deg ;   		// degree
} Tree ;


struct Point {
    int x;
    int y;
    int l;

    Point() : x(0), y(0), l(0) {} // Default constructor

    // Constructor
    Point(int x, int y, int l) : x(x), y(y), l(l) {}

    // Equality operator
    bool operator==(const Point& p2) const {
        return (x == p2.x) && (y == p2.y) && (l == p2.l);
    }

    // Inequality operator
    bool operator!=(const Point& p2) const {
        return !(*this == p2);
    }
};  

struct Pair{
    Point p;
    Point q;

    // Constructor for Pair
    Pair(const Point& p1, const Point& p2) : p(p1),q(p2) {}
};

extern int num_thread;