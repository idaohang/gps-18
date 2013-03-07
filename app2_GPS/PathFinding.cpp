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

	void	PathFinding::resolve(PathFinding::PathMode mode)
{
	NodeQueue queue;

	_begin->setWeigth(0);
	_begin->setPrevLink(0);
	queue.push(_begin);
	this->algo(queue, mode);
	this->createNodeList();
	this->reset(_begin);
}

void	PathFinding::algo(NodeQueue &queue, PathFinding::PathMode mode) const
{
	while (!queue.empty())
	{
		Node *currentNode = queue.top();
		queue.pop();
		if (currentNode == _end)
			break;
		Link	*currentLink = currentNode->getPrevLink();
		std::list<Link> &links = currentNode->getLinks();
		for (std::list<Link>::iterator it = links.begin(); it != links.end(); ++it)
		{
			Link &curLink = *it;
			double time;
			if (mode == PathFinding::FASTEST)
				time = currentNode->getWeigth() + curLink.distance / curLink.road->getSpeed();
			else
				time = currentNode->getWeigth() + curLink.distance;
			if (curLink.node->getWeigth() < 0 || curLink.node->getWeigth() > time)
			{
				curLink.node->setWeigth(time);
				curLink.node->setPrevLink(&curLink);
				curLink.prevLink = currentLink;
				queue.push(curLink.node);
			}
		}
	}
}

std::deque<Link *> &PathFinding::getResult()
{
	return _result;
}

void	PathFinding::createNodeList()
{
	_result.clear();
	Link *curLink = _end->getPrevLink();

	while (curLink != 0)
	{
		_result.push_front(curLink);
		curLink = curLink->prevLink;
	}
}

void	PathFinding::reset(Node *node)
{
	node->setWeigth(-1.0);
	std::list<Link> &links = node->getLinks();
	for (std::list<Link>::iterator it = links.begin(); it != links.end(); ++it)
	{
		Link &curLink = *it;
		if (curLink.node->getWeigth() >= 0)
			this->reset(curLink.node);
	}
}
