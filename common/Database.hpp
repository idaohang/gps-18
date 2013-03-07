#pragma once

#include <list>
#include <map>
#include "Singleton.hpp"
#include "SQLiteDatabase.hpp"
#include "Road.hpp"

class Database : public SQLiteDatabase, public Singleton<Database>
{
public:
	Database();
	virtual ~Database();

	bool		init(std::string const &path);
	void		load();
	int64_t		addRoad(Road &road, bool addNode = true);
	int64_t		addNode(Node &node, bool addLink = true);
	int64_t		addLink(Node &node, Link &link);

	std::map<int64_t, Road*>			roads;
	std::map<int64_t, Node*>			nodes;
	std::map<int64_t, std::list<Link>>	links;

private:
	void		loadRoad();
	void		loadNode();
	void		loadLink();
};