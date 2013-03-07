#include "Road.hpp"
#include "Link.hpp"

Road::Road(std::string const &name, int speed)
	: _name(name), _speed(speed), _id(-1)
{
}

Road::~Road()
{
}

void					Road::addNode(Node &node)
{
	for (auto it = _nodes.begin(); it != _nodes.end(); it++)
	{
		if (*it == &node)
			return;
	}
	_nodes.push_back(&node);
}

void					Road::addLink(Node &node1, Node &node2, double scale)
{
	node1.addLink(node2, scale, this);
	node2.addLink(node1, scale, this);
	_nodes.push_back(&node1);
	_nodes.push_back(&node2);
}

std::string const		&Road::getName() const
{
	return _name;
}

int						Road::getSpeed() const
{
	return _speed;
}

std::list<Node*> const	&Road::getNodes() const
{
	return _nodes;
}

int64_t					Road::getId() const
{
	return _id;
}

void					Road::setName(std::string const &name)
{
	_name = name;
}

void					Road::setSpeed(int speed)
{
	_speed = speed;
}

void					Road::setId(int64_t id)
{
	_id = id;
}