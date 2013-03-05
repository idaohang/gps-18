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
	queue.push(_begin);
	this->algo(queue, mode);
	this->createNodeList();
}

void	PathFinding::algo(NodeQueue &queue, PathFinding::PathMode mode) const
{
	bool	isEnd = false;
	while (!queue.empty() && !isEnd)
	{
		Node *currentNode = queue.top();
		if (currentNode == _end)
			break;
		Link	*currentLink = currentNode->getPrevLink();
		queue.pop();
		std::list<Link> &links = currentNode->getLinks();
		for (std::list<Link>::iterator it = links.begin(); it != links.end(); ++it)
		{
			Link &curLink = *it;
			double time;
			if (mode == PathFinding::FASTEST)
				time = currentNode->getWeigth() + curLink.distance / curLink.road->getSpeed();
			else
				time = currentNode->getWeigth() + curLink.distance;
			if (curLink.node.getWeigth() < 0 || curLink.node.getWeigth() > time)
			{
				curLink.node.setWeigth(time);
				curLink.node.setPrevLink(&curLink);
				curLink.prevLink = currentLink;
				queue.push(&curLink.node);
			}
		}
	}
}

std::deque<Link *> const &PathFinding::getResult() const
{
	return _result;
}

void	PathFinding::createNodeList()
{
	Link *curLink = _end->getPrevLink();

	while (curLink != 0)
	{
		_result.push_front(curLink);
		curLink = curLink->prevLink;
	}
	//Node *currentNode = _begin;
	//if (currentNode->getWeigth() < 0)
	//	return ;
	//while (currentNode != _end)
	//{
	//	_result.push_back(currentNode);
	//	Node *nextNode = 0;
	//	std::list<Link> &links = currentNode->getLinks();
	//	for (std::list<Link>::const_iterator it = links.begin(); it != links.end(); ++it)
	//	{
	//		Link const &curLink = *it;
	//		if (curLink.node.getWeigth() >= 0 && (!nextNode || curLink.node.getWeigth() < nextNode->getWeigth()))
	//			nextNode = &curLink.node;
	//	}
	//	currentNode = nextNode;
	//}
	//_result.push_back(currentNode);
}
