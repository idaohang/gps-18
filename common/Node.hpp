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
	int			distance(Node &node, double scale) const;
	double		getWeigth() const;

	void		setX(double x);
	void		setY(double y);
	void		setPosition(double x, double y);
	void		setWeigth(double weigth);

	std::list<Link>	&getLinks();

private:
	double				_x;
	double				_y;
	std::list<Link>		_links;

	// for dijkstra algorithm
	double				_weigth;
};