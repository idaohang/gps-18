#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QtGui>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <deque>
#include <iostream>
#include <string>
#include "Road.hpp"
#include "Database.hpp"
#include "myqgraphicsellipseitem.h"

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent = 0);
    Road *FinishRoadCreation(std::string const &, int);
    void CancelRoadCreation();

protected:
    void mouseReleaseEvent(QMouseEvent * event);

signals:
    
public slots:

public:
    bool isRoadDrawing;
    bool isBothWay;
    std::deque<Node*>   nodes;
    MyQGraphicsEllipseItem                  *selected;

private:
    QBrush  node;
    QPen    nodePen;
    QPen    linePen;
    std::deque<MyQGraphicsEllipseItem *>    points;
    std::deque<QGraphicsLineItem *>         lines;
};

#endif // MYGRAPHICSVIEW_H
