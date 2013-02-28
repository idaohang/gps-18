#include "Link.hpp"
#include "Node.hpp"
#include "Road.hpp"

Link::Link(Node &node, Node &node2, Road *road)
	: node(node), node2(node2), road(road)
{
	double		x = node.x - link.x;
	double		y = node.y - link.y;
	distance = x * x + y * y;
}