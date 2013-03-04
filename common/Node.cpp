#include "Node.hpp"
#include "Road.hpp"

Node::Node(double x, double y)
	: _x(x), _y(y), _weigth(-1.0)
{
}

Node::~Node()
{
}

void		Node::addLink(Link const &link)
{
	_links.push_back(link);
}

void		Node::addLink(Node &link, Road *road)
{
	_links.push_back(Link(link, this->distance(link), road));
}

double		Node::getX() const
{
	return _x;
}

double		Node::getY() const
{
	return _y;
}

double		Node::getWeigth() const
{
	return _weigth;
}

double		Node::distance(Node &node) const
{
	double	x = _x - node._x;
	double	y = _y - node._y;

	return x * x + y * y;
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

void		Node::setWeigth(double weigth)
{
	_weigth = weigth;
}
