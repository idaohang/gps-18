#include "car.h"

car::car() : speed(0), x(0.), y(0.), orientation(0), moveDistance(5)
{
    this->engine.setSingleShot(false);
    connect(&this->engine, SIGNAL(timeout()), this, SLOT(traveled()));
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
    this->engine.start(100);
}

void                    car::stopMoving()
{
    this->engine.stop();
}

void                    car::setSpeed(quint16 speed)
{
    this->speed = speed;
}

quint16                 car::getSpeed() const
{
    return this->speed;
}

// bouuuuhouhouhouhouhou
void                    car::traveled()
{
    emit moved();
    emit distanceTraveled((this->speed > 0) ? (36000 / (1000 * this->speed)) : (0));
}

void                    car::setMoveDistance(quint16 move)
{
    this->moveDistance = move;
}

quint16                 car::getMoveDistance() const
{
    return this->moveDistance;
}
