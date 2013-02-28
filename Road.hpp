#pragma once

#include <list>
#include <string>
#include "Link.hpp"

class Road
{
public:
	Road(std::string const &name = "");
	~Road();

	void				addLink(Node &node1, Node &node2);

	std::string const	getName() const;
	void				setName(std::string const &name);

private:
	std::string			_name;
	std::list<Link>		_links;
};