#ifndef CAR_H
#define CAR_H

#include <QObject>
#include <QTimer>
#include <QPair>
#include "Node.hpp"

class car : public QObject
{
    Q_OBJECT

    double                  speed; // km per hour, no miles allowed
    double                  x;
    double                  y;
    quint16                 orientation;
    QTimer                  engine;
    quint8                  moveDistance;
    Node                    *previousNode;
    Node                    *nextNode;
    bool                    moving;
private slots:
    void                    traveled();
public:
    car();
    void                    setX(double);
    void                    setY(double);
    void                    setPos(double, double);
    void                    setPos(QPair<double, double>);
    double                  getX() const;
    double                  getY() const;
    QPair<double, double>   getPos();
    quint16                 getOrientation()const;
    void                    startMoving();
    void                    stopMoving();
    void                    setSpeed(double);
    double                  getSpeed() const;
    void                    setMoveDistance(quint16);
    quint16                 getMoveDistance() const;
    void                    setNodes(Node *, Node *);
    Node                    *getPreviousNode();
    Node                    *getNextNode();
    bool                    isMoving() const;
signals:
    void                    distanceTraveled(double);
    void                    moved();
};

#endif // CAR_H
