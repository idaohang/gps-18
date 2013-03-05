#include "Node.hpp"
#include "Road.hpp"
#include <cmath>

Node::Node(double x, double y)
	: _x(x), _y(y), _weigth(-1.0), _prevLink(0)
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

Link		*Node::getPrevLink()
{
	return _prevLink;
}

double			Node::distance(Node &node, double scale) const
{
	double	x = _x - node._x;
	double	y = _y - node._y;

	return ::sqrt(x * x + y * y) * scale;
}

std::list<Link> const	&Node::getLinks() const
{
	return _links;
}

bool					Node::hasLinkTo(Node &node, Link *link) const
{
	for (auto it = _links.begin(); it != _links.end(); it++)
	{
		if (&node == it->node)
		{
			if (link)
				*link = *it;
			return true;
		}
	}
	return false;
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

void		Node::setPrevLink(Link *link)
{
	_prevLink = link;
}

std::list<Link>	&Node::getLinks()
{
	return _links;
}

bool		Node::operator<(Node &node)
{
	return (this->_weigth < node.getWeigth());
}
