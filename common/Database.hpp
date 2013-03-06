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
	bool		addRoad(Road &road, Graph &graph);

private:
	void		loadRoad(Graph &graph);
	void		loadNode(Graph &graph);
	void		loadLink(Graph &graph);
};