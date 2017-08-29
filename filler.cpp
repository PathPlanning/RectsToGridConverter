#include "filler.h"

Filler::Filler(Map &map)
{
    for (auto elem : map.obstacles) {
        create_edges(elem, map);
        process_et(map);
    }
    std::cout << "Filling completed" << std::endl;
    create_xml(map);
}

Filler::~Filler()
{
    if (!edge_table_.empty()) edge_table_.erase(edge_table_.begin(), edge_table_.end());
    if (!active_list_.empty()) active_list_.erase(active_list_.begin(), active_list_.end());
}

void Filler::create_edges(std::vector<Point<int> > obstacle, Map &map)
{
    obstacle.push_back(obstacle[0]);
    for (int i = 0; i < obstacle.size() - 1; ++i) {
        for (auto elem : lineBresenham(obstacle[i], obstacle[i + 1]))
            if (map.in_bounds(elem)) map.grid[elem.y][elem.x] = 1;
        if (obstacle[i].y != obstacle[i + 1].y) {
            double sign = static_cast<double>(obstacle[i].x - obstacle[i + 1].x) / (obstacle[i].y - obstacle[i + 1].y);
            if (sign < 0) sign = -1;
            if (sign > 0) sign = 1;
            if (obstacle[i].y < obstacle[i + 1].y) {
                edge_table_.push_back(Edge_bucket(obstacle[i].y, obstacle[i + 1].y, obstacle[i].x, sign, abs(obstacle[i + 1].x - obstacle[i].x), obstacle[i + 1].y - obstacle[i].y));
            } else if (obstacle[i].y > obstacle[i + 1].y) {
                edge_table_.push_back(Edge_bucket(obstacle[i + 1].y, obstacle[i].y, obstacle[i + 1].x, sign, abs(obstacle[i + 1].x - obstacle[i].x), obstacle[i].y - obstacle[i + 1].y));
            }
        }
    }
    edge_table_.sort([](const Edge_bucket& a, const Edge_bucket& b) { return a.ymin < b.ymin; });
}

void Filler::process_et(Map &map) {
    int scanline = edge_table_.front().ymin;
    while (!edge_table_.empty()) {
        if (!active_list_.empty()) {
            active_list_.remove_if([scanline](Edge_bucket curr) { return curr.ymax == scanline; });
            edge_table_.remove_if([scanline](Edge_bucket curr) { return curr.ymax == scanline; });
        }
        for (auto it = edge_table_.begin(); it != edge_table_.end(); ++it) {
            if (it->ymin == scanline) {
                active_list_.push_back(*it);
            }
        }
        active_list_.sort([](const Edge_bucket& a, const Edge_bucket& b) {return a.x < b.x;});
        auto it = active_list_.begin();
        while (it != active_list_.end()) {
            int from = it->x;
            ++it;
            int to = it->x;
            for (int cx = from; cx <= to; ++cx) {
                if (map.in_bounds(Point<int>(cx, scanline))) map.grid[scanline][cx] = 1;
            }
            if (it == active_list_.end()) break;
            ++it;
        }
        ++scanline;
        for (auto it = active_list_.begin(); it != active_list_.end(); ++it) {
            if (it->dx != 0) {
                it->sum += it->dx;
                while (it->sum >= it->dy) {
                    it->x += it->sign;
                    it->sum -= it->dy;
                }
            }
        }

    }
}
