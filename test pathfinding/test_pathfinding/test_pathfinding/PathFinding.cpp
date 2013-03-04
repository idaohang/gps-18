#include "PathFinding.hpp"
#include "Road.hpp"


PathFinding::PathFinding()
{
}


PathFinding::~PathFinding()
{
}

void	PathFinding::resolve() const
{
	std::deque<Node *> queue;

	queue.push_back(_end);
	this->algo(_begin, _end, queue, 0);
}

void	PathFinding::algo(Node *begin, Node *end, std::deque<Node *> &queue, double currentWeigth) const
{
	Node *currentNode = queue.front();
	queue.pop_front();

	currentNode->setWeigth(currentWeigth);
	std::list<Link> &links = currentNode->getLinks();
	for (std::list<Link>::const_iterator it = links.begin(); it != links.end(); ++it)
	{
		Link const &curLink = *it;
		double time = curLink.distance / curLink.road->getSpeed();
	}
}
