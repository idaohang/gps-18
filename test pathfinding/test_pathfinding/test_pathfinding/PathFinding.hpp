#pragma once

#include <queue>
#include <deque>
#include "Node.hpp"

class PathFinding
{
public:
	PathFinding();
	~PathFinding();

	void	setBegin(Node *node);
	void	setEnd(Node *node);
	void	resolve();
	std::deque<Link *> const &getResult() const;

private:
	Node	*_begin;
	Node	*_end;
	std::deque<Link *> _result;


	typedef std::priority_queue<Node *, std::vector<Node*>, std::less<std::vector<Node *>::value_type>> NodeQueue;

	void	algo(NodeQueue &queue) const;
	void	createNodeList();
};

