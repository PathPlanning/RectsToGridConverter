#ifndef MAP_H
#define MAP_H

#include <vector>
#include "parser.h"

struct Edges
{
    double left, right, up, down;

    Edges(double l, double r, double u, double d) : left(l), right(r), up(u), down(d) {}
};

class Map
{
public:
    Map(char *filename, double seed);
    ~Map();
    Point<int> convert_abs_point(Point<double> abs_point, Edges new_edge);
    void desc();
    Point<int> nearest_int(Point<int> point);
    bool in_bounds(Point<int> point);

    int ** grid;
    std::vector<std::vector<Point<int> > > obstacles;
    int height, width;
    Point<int> start, goal;
    double seed;
    std::string logfilename;
    bool start_point;

    const char * title;
    std::vector<std::vector<Point<double> > >  abs_obstacles;
    double abs_height, abs_width;

    Point<double> abs_start, abs_goal;
};

#endif // MAP_H
