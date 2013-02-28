#include "Node.hpp"
#include "Road.hpp"
#include <math>

Node::Node(double x, double y)
	: _x(x), _y(y)
{
}

Node::~Node()
{
}

void		Node::addLink(Link const &link)
{
	if (&link.node == this)
		_links.push_back(link);
	else if (&link.node2 == this)
		_links.push_back(link);
}

double		Node::getX() const
{
	return _x;
}

double		Node::getY() const
{
	return _y;
}

void		Node::setX(double x)
{
	_x = x;
}

void		Node::setY(double y)
{
	_y = y;
}

void		Node::setPosition(double x, double y)
{
	this->setX(x);
	this->setY(y);
}