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
#include "../../common/Road.hpp"

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
    QPen    nodePen;
    QPen    linePen;
    QPen    secondLinePen;
    std::deque<QGraphicsEllipseItem *>  points;
    std::deque<QGraphicsLineItem *>  lines;
    std::deque<QGraphicsLineItem *>  secondLines;

};

#endif // MYGRAPHICSVIEW_H
