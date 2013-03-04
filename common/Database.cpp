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
	ret = this->oneStepRequest("CREATE TABLE IF NOT EXISTS links (id INTEGER PRIMARY KEY, idNode1 INTEGER, idNode2 INTEGER, idRoad INTEGER, oneway INTEGER, distance INTEGER)");
	if (ret == false) return false;
	return ret;
}

bool		Database::addRoad(Road &road)
{
	std::string		request = "INSERT INTO roads (, ";

	request += road.getName() + ", " + Converter::toString(road.getSpeed()) + ")";

	return this->oneStepRequest(request);
}