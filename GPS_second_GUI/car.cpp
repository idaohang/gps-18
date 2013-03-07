#include "car.h"

car::car() : speed(0), x(0.), y(0.), orientation(0), moveDistance(5), previousNode(NULL), nextNode(NULL), moving(false)
{
    this->engine.setSingleShot(false);
    connect(&this->engine, SIGNAL(timeout()), this, SLOT(traveled()));
}

bool                    car::isMoving() const
{
    return moving;
}

void                    car::setX(double x)
{
    this->x = x;
    emit moved();
}

void                    car::setY(double y)
{
    this->y = y;
    emit moved();
}

void                    car::setPos(double x, double y)
{
    this->x = x;
    this->y = y;
    emit moved();
}

void                    car::setPos(QPair<double, double> pos)
{
    this->x = pos.first;
    this->y = pos.second;
    emit moved();
}

double                  car::getX() const
{
    return this->x;
}

double                  car::getY() const
{
    return this->y;
}

QPair<double, double>   car::getPos()
{
    return qMakePair(this->x, this->y);
}

quint16                 car::getOrientation()const
{
    return this->orientation;
}

void                    car::startMoving()
{
    this->moving = true;
    this->engine.start(100);
    emit moved();
}

void                    car::stopMoving()
{
    this->moving = false;
    this->engine.stop();
    emit moved();
}

void                    car::setSpeed(double speed)
{
    this->speed = speed;
    emit moved();
}

double                  car::getSpeed() const
{
    if (this->moving == false)
        return 0;
    return this->speed;
}

void                    car::traveled()
{
    emit moved();
    emit distanceTraveled((this->speed > 0) ? ((1000 * (double)this->speed) / 36000) : (0));
}

void                    car::setMoveDistance(quint16 move)
{
    this->moveDistance = move;
}

quint16                 car::getMoveDistance() const
{
    return this->moveDistance;
}

void                    car::setNodes(Node *previousNode, Node *nextNode)
{
    if (previousNode != NULL)
    {
        this->previousNode = previousNode;
    }
    if (nextNode != NULL)
    {
        this->nextNode = nextNode;
    }
}

Node                    *car::getNextNode()
{
    return this->nextNode;
}

Node                    *car::getPreviousNode()
{
    return this->previousNode;
}
