#ifndef MYQGRAPHICSELLIPSEITEM_H
#define MYQGRAPHICSELLIPSEITEM_H

#include <QGraphicsEllipseItem>
#include "Node.hpp"

class MyQGraphicsEllipseItem : public QGraphicsEllipseItem
{
public:
    MyQGraphicsEllipseItem(int a, int b, int c, int d, Node *node);

    Node *node;
};

#endif // MYQGRAPHICSELLIPSEITEM_H
