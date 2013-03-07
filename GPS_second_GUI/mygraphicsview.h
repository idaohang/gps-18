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

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent = 0);
    void FinishRoadCreation(std::string const &, int);
    void CancelRoadCreation();

protected:
    void mousePressEvent(QMouseEvent * event);

signals:
    
public slots:

public:
    bool isRoadDrawing;

public:
    QBrush  node;
    QBrush  beginBrush;
    QBrush  endBrush;
    QPen    nodePen;
    QPen    beginPen;
    QPen    endPen;
    QPen    linePen;
    QPen    secondLinePen;
    std::deque<QGraphicsEllipseItem *>  points;
    QGraphicsEllipseItem *beginPoint;
    QGraphicsEllipseItem *endPoint;
    std::deque<QGraphicsLineItem *>  lines;
    std::deque<QGraphicsLineItem *>  secondLines;

};

#endif // MYGRAPHICSVIEW_H
