#pragma once

#include <list>
#include "Singleton.hpp"
#include "SQLiteDatabase.hpp"
#include "Road.hpp"

class Database : public SQLiteDatabase, public Singleton<Database>
{
public:
	Database();
	virtual ~Database();

	bool		init(std::string const &path);
	bool		addRoad(Road &road);
};