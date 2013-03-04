#include "PathFinding.hpp"
#include "Road.hpp"


PathFinding::PathFinding()
	: _begin(0), _end(0)
{
}


PathFinding::~PathFinding()
{
}

void	PathFinding::setBegin(Node *node)
{
	_begin = node;
}

void	PathFinding::setEnd(Node *node)
{
	_end = node;
}

	void	PathFinding::resolve()
{
	NodeQueue queue;

	queue.push(_end);
	this->algo(queue);
	this->createNodeList();
}

void	PathFinding::algo(NodeQueue &queue) const
{
	while (!queue.empty())
	{
		Node *currentNode = queue.top();
		queue.pop();
		std::list<Link> &links = currentNode->getLinks();
		for (std::list<Link>::const_iterator it = links.begin(); it != links.end(); ++it)
		{
			Link const &curLink = *it;
			double time = currentNode->getWeigth() + static_cast<double>(curLink.distance) / curLink.road->getSpeed();
			if (curLink.node.getWeigth() < 0 || curLink.node.getWeigth() > time)
			{
				curLink.node.setWeigth(time);
				queue.push(&curLink.node);
			}
		}
	}
}

std::deque<Node *> const &PathFinding::getResult() const
{
	return _result;
}

void	PathFinding::createNodeList()
{
	Node *currentNode = _begin;
	if (currentNode->getWeigth() < 0)
		return ;
	while (currentNode != _end)
	{
		_result.push_back(currentNode);
		Node *nextNode = 0;
		std::list<Link> &links = currentNode->getLinks();
		for (std::list<Link>::const_iterator it = links.begin(); it != links.end(); ++it)
		{
			Link const &curLink = *it;
			if (curLink.node.getWeigth() >= 0 && (!nextNode || curLink.node.getWeigth() < nextNode->getWeigth()))
				nextNode = &curLink.node;
		}
		currentNode = nextNode;
	}
	_result.push_back(currentNode);
}
