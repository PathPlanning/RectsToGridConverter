#ifndef BRESENHAM_H
#define BRESENHAM_H

#include<stdio.h>

static const int
PIXEL_SIZE = 1000,
GAP_SIZE = 1,
WIN_SIZE = 500,
RDIM = WIN_SIZE / (PIXEL_SIZE + GAP_SIZE);

template <typename Cell>
std::vector<Cell> lineBresenham(Cell a0, Cell a1) {
    int dx, dy;
    int stepx, stepy;
    std::vector<Cell> result;

    dx = a1.x - a0.x;
    dy = a1.y - a0.y;

    if (dy<0) {dy=-dy; stepy=-1;} else {stepy=1;}
    if (dx<0) {dx=-dx; stepx=-1;} else {stepx=1;}
    dy <<= 1;
    dx <<= 1;

    if ((0 <= a0.x) && (a0.x < RDIM) && (0 <= a0.y) && (a0.y < RDIM))
        result.push_back(a0);

    if (dx > dy) {
        int fraction = dy - (dx >> 1);
        while (a0.x != a1.x) {
            a0.x += stepx;
            if (fraction >= 0) {
                a0.y += stepy;
                fraction -= dx;
            }
            fraction += dy;
            if ((0 <= a0.x) && (a0.x < RDIM) && (0 <= a0.y) && (a0.y < RDIM))
                result.push_back(a0);

        }

    } else {
        int fraction = dx - (dy >> 1);
        while (a0.y != a1.y) {
            if (fraction >= 0) {
                a0.x += stepx;
                fraction -= dy;
            }
            a0.y += stepy;
            fraction += dx;
            if ((0 <= a0.x) && (a0.x < RDIM) && (0 <= a0.y) && (a0.y < RDIM))
                result.push_back(a0);
        }
    }

    return result;
}

#endif // BRESENHAM_H
