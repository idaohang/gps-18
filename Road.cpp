#include "Road.hpp"
#include "Link.hpp"

Road::Road(std::string const &name, int speed)
	: _name(name), _speed(speed)
{
}

Road::~Road()
{
}

void					Road::addLink(Node &node1, Node &node2)
{
	node1.addLink(node2, this);
	node2.addLink(node1, this);
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

void					Road::setName(std::string const &name)
{
	_name = name;
}

void					Road::setSpeed(int speed)
{
	_speed = speed;
}