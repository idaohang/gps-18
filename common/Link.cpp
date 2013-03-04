#include "Link.hpp"
#include "Node.hpp"
#include "Road.hpp"

Link::Link(Node &node, int distance, Road *road)
	: node(node), road(road), distance(distance)
{
}