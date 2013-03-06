#ifndef ROAD_H
#define ROAD_H

#include <deque>

struct coord
{
    double x;
    double y;
};

class Road
{
public:
    Road();
    void AddNode(double x, double y);

private:
    std::deque<coord>   nodes;
};

#endif // ROAD_H
