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

bool		Database::addRoad(Road &road)
{
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
		str = "INSERT INTO nodes VALUES (null, ";
		this->oneStepRequest(Converter::toString((*it)->getX()) + ", " + Converter::toString((*it)->getY()) + ")");
		nodes.push_back(std::make_pair(*it, Converter::toString(this->getLastInsertRowID())));
	}

	for (size_t i = 0; i < nodes.size() - 1; ++i)
	{
		if (nodes[i].first->hasLinkTo(*nodes[i + 1].first, &link))
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