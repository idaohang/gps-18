#include "Road.hpp"

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

	node1.addLink(link);
	node2.addLink(link);
	_links.push_back(link);
}

std::string const		&Road::getName() const
{
	return _name;
}

void					Road::setName(std::string const &name)
{
	_name = name;
}