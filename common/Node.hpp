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

	double					getX() const;
	double					getY() const;
	int						distance(Node &node, double scale) const;
	double					getWeigth() const;
	std::list<Link> const	&getLinks() const;
	bool					hasLinkTo(Node &node, Link *link = nullptr) const;

	void		setX(double x);
	void		setY(double y);
	void		setPosition(double x, double y);
	void		setWeigth(double weigth);

private:
	double				_x;
	double				_y;
	std::list<Link>		_links;

	// for dijkstra algorithm
	double				_weigth;
};