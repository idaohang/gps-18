#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QtGui>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <deque>
#include <iostream>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent * event);

signals:
    
public slots:
    void Mouse_Pressed();
    void beginRoadDrawing();

public:
    bool isRoadDrawing;

private:
    QBrush  node;
    QPen    pen;
    std::deque<QGraphicsEllipseItem *>  points;
    std::deque<QGraphicsLineItem *>  lines;

};

#endif // MYGRAPHICSVIEW_H
