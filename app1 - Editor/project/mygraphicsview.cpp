#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
}

void MyGraphicsView::mousePressEvent(QMouseEvent * event)
{
    QGraphicsView::mousePressEvent(event);
}

void MyGraphicsView::Mouse_Pressed()
{

}

void MyGraphicsView::beginRoadDrawing()
{


}
