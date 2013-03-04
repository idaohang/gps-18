#include "Link.hpp"
#include "Node.hpp"
#include "Road.hpp"

Link::Link(Node &node, double distance, Road *road)
	: node(node), road(road), distance(distance)
{
}