#pragma once

class Road;
class Node;

class Link
{
public:
	Link(Node &node, Node &node2, Road *road = nullptr);

	Node		&node;
	Node		&node2;
	Road		*road;
	double		distance;
};