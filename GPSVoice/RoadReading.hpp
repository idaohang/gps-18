#ifndef ROADREADING_HPP
#define ROADREADING_HPP


#include <iostream>
#include <deque>
#include <QtCore/qmath.h>
#include "../common/Node.hpp"

static const qreal PI = 3.14159265358979323846264338327950288;

enum DirectionNext {
    STRAIGHT_150,
    STRAIGHT_100,
    STRAIGHT_50,
    STRAIGHT,
    TURN_LEFT_150,
    TURN_LEFT_100,
    TURN_LEFT_50,
    TURN_LEFT,
    TURN_RIGHT_150,
    TURN_RIGHT_100,
    TURN_RIGHT_50,
    TURN_RIGHT,
    TURN_BACK,
    ARRIVE,
    VOID
};

class RoadReading
{
public:
    RoadReading( std::deque<Link *> const listRoad);
    ~RoadReading();

    void            setIndex( int );
    void            reading();
    void            setNewRoad(std::deque<Link *>);
    DirectionNext   checkNextDirection();
    DirectionNext   update(double, double, double, double);
private:
    std::deque<Link *>  _listRoad;
    int                 _index;
    int                 _positionCarX;
    int                 _positionCarY;
    int                 _positionCarXOld;
    int                 _positionCarYOld;
};

#endif // ROADREADING_HPP
