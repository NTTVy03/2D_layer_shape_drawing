#pragma once

class Point {
public:
    int x;
    int y;

    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return (x == other.x) && (y == other.y);
    }
};