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

void		Database::load(Graph &graph)
{
	this->loadRoad(graph);
	this->loadNode(graph);
	this->loadLink(graph);
}

bool		Database::addRoad(Road &road, Graph &graph)
{
	if (road.getId() != -1)
		return false;

	std::string									str = "INSERT INTO roads VALUES (null, '";
	std::vector<std::pair<Node*, std::string>>	nodes;
	std::string									idRoad;
	bool										oneway;
	Link										link;

	this->oneStepRequest(str + road.getName() + "', " + Converter::toString(road.getSpeed()) + ")");
	str = "INSERT INTO nodes VALUES (null, ";
	idRoad = Converter::toString(this->getLastInsertRowID());

	for (auto it = road.getNodes().begin(); it != road.getNodes().end(); it++)
	{
		if ((*it)->getId() != -1)
			continue;
		str = "INSERT INTO nodes VALUES (null, ";
		this->oneStepRequest(Converter::toString((*it)->getX()) + ", " + Converter::toString((*it)->getY()) + ")");
		nodes.push_back(std::make_pair(*it, Converter::toString(this->getLastInsertRowID())));
	}

	for (size_t i = 0; i < nodes.size() - 1; ++i)
	{
		if (nodes[i].first->hasLinkTo(*nodes[i + 1].first, &link) && link.id == -1)
		{
			if (nodes[i + 1].first->hasLinkTo(*nodes[i].first))
				oneway = false;
			else
				oneway = true;
			str = "INSERT INTO links VALUES (null, ";
			this->oneStepRequest(str + nodes[i].second + ", " + nodes[i + 1].second + ", " + 
				idRoad + ((oneway) ? ", 1, " : ", 0, ") + Converter::toString(link.distance) + ")");
		}
	}

	return true;
}

void		Database::loadRoad(Graph &graph)
{
	auto	request = this->request("SELECT * FROM roads");
	Road	*road;

	while (request->next())
	{
		road = new Road;
		road->setId(request->get<int64_t>(0));
		road->setName(request->get<std::string>(1));
		road->setSpeed(request->get<int>(2));
		graph.roads[road->getId()] = road;
		std::cout << "Road id: " << road->getId() << " name: " << road->getName() << " speed: " << road->getSpeed() << std::endl;
	}
}

void		Database::loadNode(Graph &graph)
{
	auto	request = this->request("SELECT * FROM nodes");
	Node	*node;

	while (request->next())
	{
		node = new Node(request->get<double>(1), request->get<double>(2));
		node->setId(request->get<int64_t>(0));
		graph.nodes[node->getId()] = node;
		std::cout << "Node id: " << node->getId() << " x: " << node->getX() << " y: " << node->getY() << std::endl;
	}
}

void		Database::loadLink(Graph &graph)
{
	auto	request = this->request("SELECT * FROM links");

	while (request->next())
	{
		Link	link;
		
		link.id = request->get<int64_t>(0);
		link.distance = request->get<double>(5);
		link.node = graph.nodes[request->get<int64_t>(1)];
		link.road = graph.roads[request->get<int64_t>(3)];
		if (!link.node)
			continue;
		link.node->addLink(link);
		graph.links[link.id].push_back(link);
		if (request->get<int>(4))
		{
			link.node = graph.nodes[request->get<int64_t>(2)];
			if (!link.node)
				continue;
			link.node->addLink(link);
			graph.links[link.id].push_back(link);
		}
	}
}