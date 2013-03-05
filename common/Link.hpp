#pragma once

class Road;
class Node;

class Link
{
public:
	Link();
	Link(Node &node, double distance = 0, Road *road = nullptr);

	Node		*node;
	Road		*road;
	double		distance;

	// variable used for the pathfinding

	Link		*prevLink;
};