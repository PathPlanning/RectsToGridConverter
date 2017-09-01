#ifndef MAP_H
#define MAP_H

#include "parser.h"
#include "point.h"

struct Edges
{
    double left, right, up, down;

    Edges(double l, double r, double u, double d) : left(l), right(r), up(u), down(d) {}
};

class Map
{
public:
    Map(double sseed = 1);
    Map(const char *filename, double seed);
    ~Map();

    int * operator [] (int i);
    const int * operator [] (int i) const;
    std::vector<std::vector<Point<int> > > get_obstacles();
    bool in_bounds(Point<int> point);
    void get_map(const char *fname);
    void create_xml();
    void process_map();

    int height, width;
private:

    int ** grid;
    std::vector<std::vector<Point<int> > > obstacles;
    Point<int> start, goal;
    double seed;
    std::string logfilename;
    bool start_point;

    const char * title;
    std::vector<std::vector<Point<double> > >  abs_obstacles;
    double abs_height, abs_width;

    Point<double> abs_start, abs_goal;

    Point<int> convert_abs_point(Point<double> abs_point, Edges new_edge);
    void discrete();
    Point<int> nearest_int(Point<int> point);


};

#endif // MAP_H
