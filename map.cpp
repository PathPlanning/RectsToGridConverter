#include "map.h"

Map::Map(char * filename, double sseed)
{
    get_map(*this, filename);
    seed = sseed;
    desc();
    std::cout << "desc completed" << std::endl;
}

Map::~Map() {
    if(grid) {
        for(int c = 0; c < height; c++) {
            delete [] grid[c];
        }
        delete[] grid;
    }
}

Point<int> Map::convert_abs_point(Point<double> abs_point, Edges new_edge)
{
    Point<int> result;
    abs_point.x -= new_edge.left;
    result.x = floor(abs_point.x / seed);

    abs_point.y -= new_edge.up;
    result.y = floor(abs_point.y / seed);
    return result;
}

Point<int> Map::nearest_int(Point<int> point)
{
    Point<int> result;
    result.x = point.x;
    result.y = point.y;
    if (point.x < 0) result.x = 0;
    if (point.x >= width) result.x = width - 1;
    if (point.y < 0) result.y = 0;
    if (point.y >= height) result.y = height - 1;
    return result;
}

bool Map::in_bounds(Point<int> point)
{
    return point.x >= 0 && point.y >= 0 && point.x < width && point.y < height;
}

void Map::desc()
{
    double left_edge = abs_start.x - seed / 2;
    if (left_edge < 0) left_edge = 0;
    double right_edge = abs_width - (abs_start.x + seed / 2);
    if (right_edge < 0) right_edge = 0;
    width = static_cast<int>(left_edge / seed) + 1 + static_cast<int>(right_edge / seed);
    left_edge = left_edge - static_cast<int>(left_edge / seed) * seed;
    right_edge = abs_width - (abs_width - right_edge + static_cast<int>(right_edge / seed) * seed);

    double up_edge = abs_start.y - seed / 2;
    if (up_edge < 0) up_edge = 0;
    double down_edge = abs_height - (abs_start.y + seed / 2);
    if (down_edge < 0) down_edge = 0;
    height = static_cast<int>(up_edge / seed) + 1 + static_cast<int>(down_edge / seed);
    up_edge = up_edge - static_cast<int>(up_edge / seed) * seed;
    down_edge = abs_height - (abs_height - down_edge + static_cast<int>(down_edge / seed) * seed);

    Edges edge_(left_edge, right_edge, up_edge, down_edge);
    start = convert_abs_point(abs_start, edge_);
    if (!in_bounds(start)) start = nearest_int(start);
    goal = convert_abs_point(abs_goal, edge_);
    if (!in_bounds(goal)) goal = nearest_int(goal);
    for (auto elem : abs_obstacles) {
        std::vector<Point<int> > ob;
        for (auto point : elem) {
            Point<int> p = convert_abs_point(point, edge_);
            ob.push_back(p);
        }
        obstacles.push_back(ob);
    }
    grid = new int * [height];
    for(int count = 0; count < height; ++count){
        grid[count] = new int [width];
        for (int i = 0; i < width; ++i) {
            grid[count][i] = 0;
        }
    }
}
