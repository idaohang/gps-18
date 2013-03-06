#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    isRoadDrawing(false),
    node(Qt::blue),
    pen(Qt::blue)
{
    pen.setWidth(2);
}

void MyGraphicsView::mousePressEvent(QMouseEvent * event)
{
    QPointF clickPos;
    QGraphicsEllipseItem *lastPoint;

    QGraphicsView::mousePressEvent(event);
    if (this->isRoadDrawing)
    {
        if (event->button() == Qt::LeftButton) // add a road point
        {
            clickPos = this->mapToScene(event->pos());
            if (!this->points.empty())
            {
                lastPoint = this->points.back();
                this->lines.push_back(this->scene()->addLine(lastPoint->scenePos().x(), lastPoint->scenePos().y(), clickPos.x(), clickPos.y(), this->pen));
            }
            this->points.push_back(this->scene()->addEllipse(-4, -4, 8, 8, this->pen, node));
            this->points.back()->setPos(clickPos);
        }
        else if (event->button() == Qt::RightButton) // remove the last point
        {
            if (!this->points.empty())
            {
                this->scene()->removeItem(this->points.back());
                this->points.pop_back();
                if (!this->lines.empty())
                {
                    this->scene()->removeItem(this->lines.back());
                    this->lines.pop_back();
                }
            }
        }
    }
}

void MyGraphicsView::Mouse_Pressed()
{

}

void MyGraphicsView::beginRoadDrawing()
{


}
