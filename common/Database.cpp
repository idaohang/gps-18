#include <vector>
#include <algorithm>
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

	this->clear();
	ret = this->open(path);
	if (ret == false) return false;
	ret = this->oneStepRequest("CREATE TABLE IF NOT EXISTS roads (id INTEGER PRIMARY KEY, name TEXT, speed INTEGER)");
	if (ret == false) return false;
	ret = this->oneStepRequest("CREATE TABLE IF NOT EXISTS nodes (id INTEGER PRIMARY KEY, x REAL, y REAL)");
	if (ret == false) return false;
	ret = this->oneStepRequest("CREATE TABLE IF NOT EXISTS links (id INTEGER PRIMARY KEY, idNode1 INTEGER, idNode2 INTEGER, idRoad INTEGER, oneway INTEGER, distance REAL)");
	if (ret == false) return false;
	ret = this->oneStepRequest("CREATE TABLE IF NOT EXISTS imgs (id INTEGER PRIMARY KEY, x INTEGER, y INTEGER, path TEXT");
	return ret;
}

void		Database::load()
{
	this->loadRoad();
	this->loadNode();
	this->loadLink();
	this->loadImage();
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
	link.id = this->getLastInsertRowID();
	link2.id = link.id;
	this->links[link.id].push_back(link);
	return link.id;
}

int64_t		Database::addImage(Image &img)
{
	if (img.id != -1)
		return img.id;
	auto	request = this->request("INSERT INTO imgs VALUES (null, ?1, ?2, ?3)");
	request->bind(img.x, img.y, img.path);
	request->next();
	img.id = this->getLastInsertRowID();
	this->images[img.id] = &img;
	return img.id;
}

void		Database::deleteRoad(Road &road)
{
	if (road.getId() == -1)
		return;
	auto	request = this->request("DELETE FROM roads WHERE id=?1");
	request->bind(road.getId());
	request->next();
	auto	i = this->roads.find(road.getId());
	if (i != this->roads.end())
		this->roads.erase(i);
	road.setId(-1);

	for (auto it = road.getNodes().begin(); it != road.getNodes().end(); it++)
		this->deleteNode(**it, &road);
}

void		Database::deleteNode(Node &node, Road *road)
{
	if (node.getId() == -1)
		return;
	bool		del = true;
	for (auto it = node.getLinks().begin(); it != node.getLinks().end(); it++)
	{
		if (it->road != road)
			del = false;
		else
			this->deleteLink(*it);
	}

	if (!del)
		return;
	auto	request = this->request("DELETE FROM nodes WHERE id=?1");
	request->bind(node.getId());
	request->next();
	auto	i = this->nodes.find(node.getId());
	if (i != this->nodes.end())
		this->nodes.erase(i);
	node.setId(-1);
}

void		Database::deleteLink(Link &link)
{
	if (link.id == -1)
		return;
	auto	request = this->request("DELETE FROM links WHERE id=?1");
	request->bind(link.id);
	request->next();
	auto	i = this->links.find(link.id);
	if (i != this->links.end())
		this->links.erase(i);
	link.id = -1;
}

void		Database::clear()
{
	std::for_each(this->roads.begin(), this->roads.end(), [] (std::pair<int64_t, Road*> pair) {delete pair.second;});
	this->roads.clear();
	std::for_each(this->nodes.begin(), this->nodes.end(), [] (std::pair<int64_t, Node*> pair) {delete pair.second;});
	this->nodes.clear();
	std::for_each(this->images.begin(), this->images.end(), [] (std::pair<int64_t, Image*> pair) {delete pair.second;});
	this->images.clear();
	this->links.clear();
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
        //std::cout << "Road id: " << road->getId() << " name: " << road->getName() << " speed: " << road->getSpeed() << std::endl;
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
        //std::cout << "Node id: " << node->getId() << " x: " << node->getX() << " y: " << node->getY() << std::endl;
	}
}

void		Database::loadLink()
{
	auto	request = this->request("SELECT * FROM links");

	while (request->next())
	{
		Link	link;
        auto    it = this->nodes.find(request->get<int64_t>(1));
        auto    it2 = this->nodes.find(request->get<int64_t>(2));
        if (it == this->nodes.end() || it2 == this->nodes.end())
            continue;
        auto    itroad = this->roads.find(request->get<int64_t>(3));
        Road    *road = nullptr;
        if (itroad != this->roads.end())
            road = itroad->second;
		if (road)
		{
			road->addNode(*it->second);
			road->addNode(*it2->second);
		}

		link.id = request->get<int64_t>(0);
		link.distance = request->get<double>(5);
        link.node = it2->second;
        link.road = road;
        it->second->addLink(link);
		this->links[link.id].push_back(link);
        if (!request->get<bool>(4))
		{
            link.node = it->second;
			if (!link.node)
				continue;
            it2->second->addLink(link);
			this->links[link.id].push_back(link);
		}
	}
}

void		Database::loadImage()
{
	auto	request = this->request("SELECT * FROM imgs");
	Image	*img;

	while (request->next())
	{
		img = new Image(request->get<std::string>(3), request->get<int>(1), request->get<int>(2));
		img->id = request->get<int64_t>(0);
		this->images[img->id] = img;
	}
}