#pragma once

#include <list>
#include "Singleton.hpp"
#include "SQLiteDatabase.hpp"
#include "Graph.hpp"

class Database : public SQLiteDatabase, public Singleton<Database>
{
public:
	Database();
	virtual ~Database();

	bool		init(std::string const &path);
	void		load(Graph &graph);
	int64_t		addRoad(Road &road, Graph &graph);
	int64_t		addNode(Node &node, Graph &graph);
	int64_t		addLink(Node &node, Link &link, Graph &graph);

private:
	void		loadRoad(Graph &graph);
	void		loadNode(Graph &graph);
	void		loadLink(Graph &graph);
};