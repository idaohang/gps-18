#ifndef CAR_H
#define CAR_H

#include <QObject>
#include <QTimer>
#include <QPair>

class car : public QObject
{
    Q_OBJECT

    quint16                 speed; // km per hour, no miles allowed
    double                  x;
    double                  y;
    quint16                 orientation;
    QTimer                  engine;
    quint8                  moveDistance;
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
    void                    setSpeed(quint16);
    quint16                 getSpeed() const;
    void                    setMoveDistance(quint16);
    quint16                 getMoveDistance() const;
signals:
    void                    distanceTraveled(quint16);
    void                    moved();
};

#endif // CAR_H
