#include "myqgraphicsellipseitem.h"

MyQGraphicsEllipseItem::MyQGraphicsEllipseItem(int a, int b, int c, int d, Node *node)
    : QGraphicsEllipseItem(a, b, c, d), node(node)
{
}
