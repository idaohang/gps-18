#include "RoadReading.hpp"

RoadReading::RoadReading( std::deque<Link *> const listRoad )
    : _listRoad( listRoad )
{
    this->_index = 0;
    this->_positionCarX = -1;
    this->_positionCarY = -1;
}

RoadReading::~RoadReading()
{
}

void        RoadReading::setIndex(int index)
{
    this->_index = index;
}

void        RoadReading::setNewRoad(std::deque<Link *> const listRoad )
{
    this->_listRoad = listRoad;
    this->_index    = 0;
}

DirectionNext         RoadReading::checkNextDirection()
{
    qreal x1 = this->_positionCarX - this->_listRoad[this->_index]->node->getX();
    qreal y1 = this->_positionCarY - this->_listRoad[this->_index]->node->getY();
    qreal x2 = this->_listRoad[this->_index + 1]->node->getX() - this->_listRoad[this->_index]->node->getX();
    qreal y2 = this->_listRoad[this->_index + 1]->node->getY() - this->_listRoad[this->_index]->node->getY();
    double angle = 180 * (qAtan2(y2, x2) - qAtan2(y1, x1)) / PI;
    if ( angle < 135 && angle > 45)
        return TURN_LEFT;
    else if ( angle < 315 && angle > 225)
        return TURN_RIGHT;
    else if ( angle <= 225 && angle >= 135)
        return STRAIGHT;
    return VOID;
}

DirectionNext           RoadReading::update(double positionCarX, double positionCarY, double scale, double counter)
{
    this->_positionCarXOld = this->_positionCarX;
    this->_positionCarYOld = this->_positionCarY;
    this->_positionCarX = positionCarX;
    this->_positionCarY = positionCarY;
    double distanceOld = 0;
    if ( this->_positionCarXOld != -1 )
    {
        double xOld = this->_positionCarXOld - this->_listRoad[this->_index]->node->getX();
        double yOld = this->_positionCarYOld - this->_listRoad[this->_index]->node->getY();
        distanceOld = ( sqrt(xOld * xOld + yOld * yOld) * scale );
    }
    double x = this->_positionCarX - this->_listRoad[this->_index]->node->getX();
    double y = this->_positionCarY - this->_listRoad[this->_index]->node->getY();
    double distance = sqrt(x * x + y * y) * scale;
    //Update end of travel
    if ( this->_index >= this->_listRoad.size() - 1 )
    {
        if ( this->_positionCarXOld != -1 && distanceOld - counter < 0.0 )
            return ARRIVE;
        return VOID;
    }
    //Update turn back
    if (  _positionCarXOld != -1 && distance > distanceOld )
        return TURN_BACK;
    //Update cross road
    if ( this->_listRoad[_index]->node->getLinks().size() <= 2 )
    {
        if ( distance >= 1 && distance - counter <= 1 && this->_index < this->_listRoad.size() - 1 )
            this->_index += 1;
        return VOID;
    }
    if ( distance < 175 )
    {

        DirectionNext   direction = checkNextDirection();
        if ( distance >= 1 && distance - counter <= 1 )
        {
            this->_index += 1;
            this->_positionCarXOld = -1;
            return direction;
        }
        if ( distance >= 50 && distance - counter <= 50 )
        {
            if ( direction == TURN_LEFT )
                return TURN_LEFT_50;
            else if ( direction == TURN_RIGHT )
                return TURN_RIGHT_50;
            else if ( direction == STRAIGHT )
                return STRAIGHT_50;
        }
        //else if ( distance >= 100 && distance - counter <= 100 )
        //{
        //    if ( direction == TURN_LEFT )
        //        return TURN_LEFT_100;
        //    else if ( direction == TURN_RIGHT )
        //        return TURN_RIGHT_100;
        //    else if ( direction == STRAIGHT )
        //        return STRAIGHT_100;
        //}
        else if ( distance >= 150 && distance - counter <= 150 )
        {
            if ( direction == TURN_LEFT )
                return TURN_LEFT_150;
            else if ( direction == TURN_RIGHT )
                return TURN_RIGHT_150;
            else if ( direction == STRAIGHT )
                return STRAIGHT_150;
        }
    }
    return VOID;
}
