#pragma once

#include <queue>
#include "Node.hpp"

class PathFinding
{
public:
	PathFinding();
	~PathFinding();

	void	resolve() const;

private:
	Node	*_begin;
	Node	*_end;

	void	algo(Node *begin, Node *end, std::deque<Node *> &queue, double currentWeigth) const;
};

