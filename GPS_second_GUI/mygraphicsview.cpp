#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    isRoadDrawing(false),
    node(QColor(0, 0, 255, 255)),
    beginBrush(QColor(0, 255, 0, 255)),
    endBrush(QColor(255, 0, 0, 255)),
    nodePen(QColor(0, 0, 255, 255)),
    beginPen(QColor(0, 255, 0, 255)),
    endPen(QColor(255, 0, 0, 255)),
    linePen(QColor(0, 0, 255, 100)),
    secondLinePen(QColor(255, 0, 0, 255)),
    beginPoint(0),
    endPoint(0)
{
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    nodePen.setWidth(0);
    linePen.setWidth(5);
    secondLinePen.setWidth(5);
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
                this->lines.push_back(this->scene()->addLine(lastPoint->scenePos().x(), lastPoint->scenePos().y(), clickPos.x(), clickPos.y(), this->linePen));

            }
            this->points.push_back(this->scene()->addEllipse(-4, -4, 8, 8, this->nodePen, node));
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

void MyGraphicsView::CancelRoadCreation()
{
    std::deque<QGraphicsEllipseItem *>::iterator it1;
    std::deque<QGraphicsLineItem *>::iterator it2;

    for (it1 = this->points.begin(); it1 != this->points.end(); ++it1)
    {
         this->scene()->removeItem(*it1);
    }
    for (it2 = this->lines.begin(); it2 != this->lines.end(); ++it2)
    {
        this->scene()->removeItem(*it2);
    }
    this->points.clear();
    this->lines.clear();
}

void MyGraphicsView::FinishRoadCreation(std::string const & name, int speed)
{
    std::deque<QGraphicsEllipseItem *>::iterator it1;
    std::deque<QGraphicsLineItem *>::iterator it2;

    Node    *lastPoint;
    Node    *currentPoint;
    Road    *road;

    road = new Road(name, speed);
    for (it1 = this->points.begin(); it1 != this->points.end(); ++it1)
    {
        if (it1 == this->points.begin())
            lastPoint = new Node((*it1)->pos().x(), (*it1)->pos().y());
        else
        {
            currentPoint = new Node((*it1)->pos().x(), (*it1)->pos().y());
            road->addLink(*lastPoint, *currentPoint, 1);
        }

        (*it1)->setOpacity(0.3);
        lastPoint = currentPoint;
    }
    for (it2 = this->lines.begin(); it2 != this->lines.end(); ++it2)
    {
        (*it2)->setOpacity(0.3);
    }

    this->points.clear();
    this->lines.clear();
}
