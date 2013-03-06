#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QtGui>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <deque>

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
    QBrush  link;
    QPen    pen;

};

#endif // MYGRAPHICSVIEW_H
