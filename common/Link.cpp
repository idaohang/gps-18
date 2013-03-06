#include "Link.hpp"
#include "Node.hpp"
#include "Road.hpp"

Link::Link()
	: node(nullptr), road(nullptr), distance(0), prevLink(0), id(-1)
{
}

Link::Link(Node &node, double distance, Road *road)
	: node(&node), road(road), distance(distance), prevLink(0), id(-1)
{
}