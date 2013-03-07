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
#include "../../common/Database.hpp"

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent = 0);
    Road *FinishRoadCreation(std::string const &, int);
    void CancelRoadCreation();

protected:
    void mousePressEvent(QMouseEvent * event);

signals:
    
public slots:

public:
    bool isRoadDrawing;
    std::deque<Node*>   nodes;

private:
    QBrush  node;
    QPen    nodePen;
    QPen    linePen;
    std::deque<QGraphicsEllipseItem *>  points;
    std::deque<QGraphicsLineItem *>  lines;


};

#endif // MYGRAPHICSVIEW_H
