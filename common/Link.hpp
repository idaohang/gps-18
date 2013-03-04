#pragma once

class Road;
class Node;

class Link
{
public:
	Link(Node &node, double distance = 0, Road *road = nullptr);

	Node		&node;
	Road		*road;
	double		distance;
};