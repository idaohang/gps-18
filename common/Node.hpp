#pragma once

#include <list>
#include "Link.hpp"

class Node
{
public:
	Node(double x = 0, double y = 0);
	~Node();

	void		addLink(Link const &link);
	void		addLink(Node &link, double scale, Road *road = nullptr);

	double		getX() const;
	double		getY() const;
	double		distance(Node &node, double scale) const;
	double		getWeigth() const;
	Link		*getPrevLink();

	void		setX(double x);
	void		setY(double y);
	void		setPosition(double x, double y);
	void		setWeigth(double weigth);
	void		setPrevLink(Link *link);

	std::list<Link>	&getLinks();

	bool		operator<(Node &node);

private:
	double				_x;
	double				_y;
	std::list<Link>		_links;

	// variable used for the pathfinding
	double				_weigth;
	Link				*_prevLink;
};