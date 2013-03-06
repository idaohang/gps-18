#include <vector>
#include "Database.hpp"
#include "Converter.hpp"
#include "Graph.hpp"

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

int64_t		Database::addRoad(Road &road)
{
	if (road.getId() != -1)
		return road.getId();

	std::string				str = "INSERT INTO roads VALUES (null, '";
	std::vector<Node*>		nodes;
	Link					link;

	this->oneStepRequest(str + road.getName() + "', " + Converter::toString(road.getSpeed()) + ")");
	str = "INSERT INTO nodes VALUES (null, ";
	road.setId(this->getLastInsertRowID());
	this->roads[road.getId()] = &road;

	for (auto it = road.getNodes().begin(); it != road.getNodes().end(); it++)
	{
		this->addNode(**it);
		nodes.push_back(*it);
	}

	for (size_t i = 0; i < nodes.size() - 1; ++i)
	{
		if (nodes[i]->hasLinkTo(*nodes[i + 1], &link))
			this->addLink(*nodes[i], link);
	}

	return road.getId();
}

int64_t		Database::addNode(Node &node)
{
	if (node.getId() != -1)
		return node.getId();
	std::string str = "INSERT INTO nodes VALUES (null, ";
	this->oneStepRequest(Converter::toString(node.getX()) + ", " + Converter::toString(node.getY()) + ")");
	int64_t		id = this->getLastInsertRowID();
	node.setId(id);
	this->nodes[id] = &node;
	return id;
}

int64_t		Database::addLink(Node &node, Link &link)
{
	if (link.id != -1)
		return link.id;
	bool	oneway;

	if (link.node->hasLinkTo(node))
		oneway = false;
	else
		oneway = true;
	std::string		str = "INSERT INTO links VALUES (null, ";
	this->oneStepRequest(str + Converter::toString(node.getId()) + ", " + Converter::toString(link.node->getId()) + ", " + 
		Converter::toString(link.road->getId()) + ((oneway) ? ", 1, " : ", 0, ") + Converter::toString(link.distance) + ")");
	link.id = this->getLastInsertRowID();
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