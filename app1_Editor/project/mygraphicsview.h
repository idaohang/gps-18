#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QtGui>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <deque>
#include <iostream>
#include "road.h"

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent = 0);
    void finishRoadCreation();
    void CancelRoadCreation();

protected:
    void mousePressEvent(QMouseEvent * event);

signals:
    
public slots:

public:
    bool isRoadDrawing;

private:
    QBrush  node;
    QPen    nodePen;
    QPen    linePen;
    std::deque<QGraphicsEllipseItem *>  points;
    std::deque<QGraphicsLineItem *>  lines;

};

#endif // MYGRAPHICSVIEW_H
