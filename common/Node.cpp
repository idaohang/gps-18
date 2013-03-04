#include "Node.hpp"
#include "Road.hpp"
<<<<<<< HEAD
#include <cmath>
=======
>>>>>>> 86e7be74dc25bafd2272c44542c526723de7c850

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

void		Node::addLink(Node &link, double scale, Road *road)
{
	_links.push_back(Link(link, this->distance(link, scale), road));
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

int			Node::distance(Node &node, double scale) const
{
	double	x = _x - node._x;
	double	y = _y - node._y;

	return static_cast<int>(::sqrt(x * x + y * y) * scale);
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

std::list<Link>	&Node::getLinks()
{
	return _links;
}
