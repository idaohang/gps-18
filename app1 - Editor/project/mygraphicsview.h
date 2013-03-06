#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QtGui>
#include <QObject>
#include <QGraphicsView>

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

};

#endif // MYGRAPHICSVIEW_H
