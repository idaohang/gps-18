#include "Node.hpp"
#include "Road.hpp"
#include "PathFinding.hpp"

int main()
{
	Node *n1 = new Node(0, 0);
	Node *n2 = new Node(5, 0);
	Node *n3 = new Node(2, 1);
	Node *n4 = new Node(2, 2);
	Node *n5 = new Node(4, 2);
	Node *n6 = new Node(1, 4);
	Node *n7 = new Node(3, 4);
	Node *n8 = new Node(5, 4);
	Node *n9 = new Node(2, 5);
	Node *n10 = new Node(4, 5);

	Road *road = new Road("youpi", 30);
	Road *road2 = new Road("youpa", 120);

	n1->addLink(*n3, 1, road);
	n3->addLink(*n1, 1, road);

	n2->addLink(*n3, 1, road);
	n3->addLink(*n2, 1, road);

	n2->addLink(*n4, 1, road);
	n4->addLink(*n2, 1, road);

	n2->addLink(*n5, 1, road);
	n5->addLink(*n2, 1, road);

	n4->addLink(*n5, 1, road);
	n5->addLink(*n4, 1, road);

	n4->addLink(*n7, 1, road);
	n7->addLink(*n4, 1, road);

	n5->addLink(*n8, 1, road);
	n8->addLink(*n5, 1, road);

	n10->addLink(*n8, 1, road);
	n8->addLink(*n10, 1, road);

	n9->addLink(*n7, 1, road2);
	n7->addLink(*n9, 1, road2);

	n9->addLink(*n6, 1, road2);
	n6->addLink(*n9, 1, road2);

	n1->addLink(*n6, 1, road2);
	n6->addLink(*n1, 1, road2);

	PathFinding path;

	path.setBegin(n4);
	path.setEnd(n1);
	path.resolve(PathFinding::FASTEST);

	{
		std::deque<Link *> const &result = path.getResult();
		int i = 0;
	}
	path.setBegin(n2);
	path.resolve(PathFinding::FASTEST);

	{
		std::deque<Link *> const &result = path.getResult();
		int i = 0;
	}
}
