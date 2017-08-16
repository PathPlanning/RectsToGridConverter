#ifndef FILLER_H
#define FILLER_H

#include "map.h"
#include "bresenham.h"
#include <queue>
#include <list>

struct Edge_bucket {
    int ymin, ymax;
    int x;
    int sign;
    int dx, dy;
    int sum;

    std::list<Edge_bucket>::iterator buddy;

    Edge_bucket(int min_, int max_, int x_, int sign_, int dx_, int dy_, int sum_ = 0) : ymin(min_), ymax(max_), x(x_), sign(sign_), dx(dx_), dy(dy_), sum(sum_) {}
};

class Filler
{
public:
    Filler(Map &map);
    void create_edges(std::vector<Point<int> > obstacle, Map &map);
    void process_et(Map &map);

private:
    std::list<Edge_bucket> edge_table_;
    std::list<Edge_bucket> active_list_;
};

#endif // FILLER_H
