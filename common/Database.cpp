#include <vector>
#include "Database.hpp"
#include "Converter.hpp"

Database::Database()
{
}

Database::~Database()
{
}

bool		Database::init(std::string const &path)
{
	bool	ret;

	ret = this->open(path);
	if (ret == false) return false;
	ret = this->oneStepRequest("CREATE TABLE IF NOT EXISTS roads (id INTEGER PRIMARY KEY, name TEXT, speed INTEGER)");
	if (ret == false) return false;
	ret = this->oneStepRequest("CREATE TABLE IF NOT EXISTS nodes (id INTEGER PRIMARY KEY, x REAL, y REAL)");
	if (ret == false) return false;
	ret = this->oneStepRequest("CREATE TABLE IF NOT EXISTS links (id INTEGER PRIMARY KEY, idNode1 INTEGER, idNode2 INTEGER, idRoad INTEGER, oneway INTEGER, distance REAL)");
	return ret;
}

void		Database::load()
{
	this->loadRoad();
	this->loadNode();
	this->loadLink();
}

int64_t		Database::addRoad(Road &road, bool addNode)
{
	if (road.getId() == -1)
	{
		auto	request = this->request("INSERT INTO roads VALUES (null, ?1, ?2)");
		request->bind(road.getName(), road.getSpeed());
		request->next();
		road.setId(this->getLastInsertRowID());
		this->roads[road.getId()] = &road;
	}

	if (addNode)
	{
		for (auto it = road.getNodes().begin(); it != road.getNodes().end(); it++)
			this->addNode(**it, false);

		for (auto it = road.getNodes().begin(); it != road.getNodes().end(); it++)
			this->addNode(**it, true);
	}

	return road.getId();
}

int64_t		Database::addNode(Node &node, bool addLink)
{
	if (node.getId() == -1)
	{
		auto	request = this->request("INSERT INTO nodes VALUES (null, ?1, ?2)");
		request->bind(node.getX(), node.getY());
		request->next();
		node.setId(this->getLastInsertRowID());
		this->nodes[node.getId()] = &node;
	}
	
	if (addLink)
	{
		for (auto it = node.getLinks().begin(); it != node.getLinks().end(); it++)
			this->addLink(node, *it);
	}

	return node.getId();
}

int64_t		Database::addLink(Node &node, Link &link)
{
	if (link.id != -1)
		return link.id;
	bool	oneway;
	Link	link2;

	if (link.node->hasLinkTo(node, &link2))
		oneway = false;
	else
		oneway = true;
	auto	request = this->request("INSERT INTO links VALUES (null, ?1, ?2, ?3, ?4, ?5)");
	request->bind(node.getId(), link.node->getId(), link.road->getId(), (oneway) ? true : false, link.distance);
	request->next();
	std::cout << request->getLastError() << std::endl;
	link.id = this->getLastInsertRowID();
	link2.id = link.id;
	this->links[link.id].push_back(link);
	return link.id;
}

void		Database::loadRoad()
{
	auto	request = this->request("SELECT * FROM roads");
	Road	*road;

	while (request->next())
	{
		road = new Road;
		road->setId(request->get<int64_t>(0));
		road->setName(request->get<std::string>(1));
		road->setSpeed(request->get<int>(2));
		this->roads[road->getId()] = road;
		std::cout << "Road id: " << road->getId() << " name: " << road->getName() << " speed: " << road->getSpeed() << std::endl;
	}
}

void		Database::loadNode()
{
	auto	request = this->request("SELECT * FROM nodes");
	Node	*node;

	while (request->next())
	{
		node = new Node(request->get<double>(1), request->get<double>(2));
		node->setId(request->get<int64_t>(0));
		this->nodes[node->getId()] = node;
		std::cout << "Node id: " << node->getId() << " x: " << node->getX() << " y: " << node->getY() << std::endl;
	}
}

void		Database::loadLink()
{
	auto	request = this->request("SELECT * FROM links");

	while (request->next())
	{
		Link	link;
		
		link.id = request->get<int64_t>(0);
		link.distance = request->get<double>(5);
        link.node = this->nodes[request->get<int64_t>(1)];
		link.road = this->roads[request->get<int64_t>(3)];
		if (!link.node)
			continue;
		link.node->addLink(link);
		this->links[link.id].push_back(link);
		if (request->get<int>(4))
		{
            link.node = this->nodes[request->get<int64_t>(2)];
			if (!link.node)
				continue;
			link.node->addLink(link);
			this->links[link.id].push_back(link);
		}
	}
}
