#include "Road.hpp"
#include "Link.hpp"

Road::Road(std::string const &name)
	: _name(name)
{
}

Road::~Road()
{
}

void					Road::addLink(Node &node1, Node &node2)
{
	Link				link(node1, node2, this);

	node1.addLink(node2, this);
	node2.addLink(node1, this);
	_nodes.push_back(&node1);
	_nodes.push_back(&node2);
}

std::string const		&Road::getName() const
{
	return _name;
}

void					Road::setName(std::string const &name)
{
	_name = name;
}