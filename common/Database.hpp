#pragma once

#include <list>
#include <map>
#include "Singleton.hpp"
#include "SQLiteDatabase.hpp"
#include "Road.hpp"
#include "Image.hpp"

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
	int64_t		addImage(Image &image);

	void		deleteRoad(Road &road);
	void		deleteNode(Node &node, Road *road = nullptr);
	void		deleteLink(Link &link);

	std::map<int64_t, Road*>			roads;
	std::map<int64_t, Node*>			nodes;
	std::map<int64_t, std::list<Link>>	links;
	std::map<int64_t, Image*>			images;

private:
	void		clear();
	void		loadRoad();
	void		loadNode();
	void		loadLink();
	void		loadImage();
};