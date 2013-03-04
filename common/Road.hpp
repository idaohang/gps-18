#pragma once

#include <list>
#include <string>
#include "Node.hpp"

class Road
{
public:
	Road(std::string const &name = "", int speed = 90);
	~Road();

	void				addLink(Node &node1, Node &node2, double scale);

	std::string const	&getName() const;
	int					getSpeed() const;

	void				setName(std::string const &name);
	void				setSpeed(int speed);

private:
	std::string			_name;
	int					_speed;
	std::list<Node*>	_nodes;
};