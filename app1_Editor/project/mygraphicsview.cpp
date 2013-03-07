#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    isRoadDrawing(false),
    node(QColor(0, 0, 255, 255)),
    nodePen(QColor(0, 0, 255, 255)),
    linePen(QColor(0, 0, 255, 150)),
    selected(0),
    isBothWay(true)
{
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    nodePen.setWidth(0);
    linePen.setWidth(6);
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent * event)
{
    QPointF clickPos;
    MyQGraphicsEllipseItem *lastPoint = 0;
    MyQGraphicsEllipseItem *item;
    QPointF pos;
    Node    *tmpnode;
    //int    itemExists = 0;

    QGraphicsView::mouseReleaseEvent(event);
    if (this->isRoadDrawing)
    {
        if (event->button() == Qt::LeftButton) // add a road point
        {
            clickPos = this->mapToScene(event->pos());
            if (!this->points.empty())
            {
                lastPoint = this->points.back();
                if (!selected)
                    this->lines.push_back(this->scene()->addLine(lastPoint->scenePos().x(), lastPoint->scenePos().y(), clickPos.x(), clickPos.y(), this->linePen));
            }
            if (selected)
            {
                selected->node->nb += 1;
                if (lastPoint)
                    this->lines.push_back(this->scene()->addLine(lastPoint->scenePos().x(), lastPoint->scenePos().y(), selected->scenePos().x(), selected->scenePos().y(), this->linePen));
                lastPoint = selected;
                this->points.push_back(lastPoint);
                selected = 0;
                return;
            }
            tmpnode = new Node(clickPos.x(), clickPos.y());
            item = new MyQGraphicsEllipseItem(-4, -4, 8, 8, tmpnode);
            item->setPen(this->nodePen);
            item->setBrush(node);
            pos.setX(tmpnode->getX());
            pos.setY(tmpnode->getY());
            item->setPos(pos);
            item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
            this->scene()->addItem(item);
            this->points.push_back(item);
            this->points.back()->setPos(clickPos);

        }
        else if (event->button() == Qt::RightButton) // remove the last point
        {
            if (!this->points.empty())
            {
                if (this->points.back()->node->nb <= 1)
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
    for (auto it1 = this->points.begin(); it1 != this->points.end(); ++it1)
    {
        if ((*it1)->node->nb <= 1)
            this->scene()->removeItem(*it1);
    }
    for (auto it2 = this->lines.begin(); it2 != this->lines.end(); ++it2)
    {
        this->scene()->removeItem(*it2);
    }
    this->points.clear();
    this->lines.clear();
}

Road *MyGraphicsView::FinishRoadCreation(std::string const & name, int speed)
{
    Node    *lastPoint;
    Node    *currentPoint;
    Road    *road;

    road = new Road(name, speed);
    for (auto it1 = this->points.begin(); it1 != this->points.end(); ++it1)
    {
        if (it1 == this->points.begin())
        {
            lastPoint = (*it1)->node;
            this->nodes.push_back(lastPoint);
        }
        else
        {
            currentPoint = (*it1)->node;
            lastPoint->addLink(*currentPoint, 1, road);
            if (this->isBothWay)
            {
                currentPoint->addLink(*lastPoint, 1, road);
            }
            this->nodes.push_back(currentPoint);
            lastPoint = currentPoint;
        }
        (*it1)->setOpacity(0.3);
    }
    /*if (this->points.back()->node->nb > 1)
    {
        for (auto it = this->nodes.begin(); it != this->nodes.end() - 1; ++it)
        {
            if (*it == this->points.back()->node)
            {
                if (this->isBothWay)
                    (*it)->addLink(*this->points.back()->node, 1, road);
                this->points.back()->node->addLink(*(*it), 1, road);
                std::cerr << this->points.back()->node << " " << *it << std::endl;
                break;
            }
        }
    }*/
    for (auto it2 = this->lines.begin(); it2 != this->lines.end(); ++it2)
    {
        (*it2)->setOpacity(0.5);
    }
    std::cerr << this->nodes.size() << std::endl;
    this->points.clear();
    this->lines.clear();
    return road;
}

