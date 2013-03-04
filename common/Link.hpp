#pragma once

class Road;
class Node;

class Link
{
public:
	Link(Node &node, int distance = 0, Road *road = nullptr);

	Node		&node;
	Road		*road;
	int			distance;
};