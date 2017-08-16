#ifndef POINT_H
#define POINT_H

template <typename T>
struct Point
{
    T x, y;

    Point(T x_ = 0, T y_ = 0) : x(x_), y(y_) {}
};



#endif // POINT_H
