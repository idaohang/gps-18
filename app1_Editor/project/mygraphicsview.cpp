#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    isRoadDrawing(false),
    node(Qt::red),
    link(Qt::blue),
    pen(Qt::black)
{
    pen.setWidth(1);
}

void MyGraphicsView::mousePressEvent(QMouseEvent * event)
{
    QGraphicsView::mousePressEvent(event);
    if (this->isRoadDrawing)
    {
        this->scene()->addEllipse(0, 0, 8, 8, this->pen, node);
    }
}

void MyGraphicsView::Mouse_Pressed()
{

}

void MyGraphicsView::beginRoadDrawing()
{


}
