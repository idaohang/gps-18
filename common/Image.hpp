#pragma once

#include <cstdint>
#include <string>

class Image
{
public:
	Image(std::string const &path = "", int x = 0, int y = 0);

	int				x;
	int				y;
	std::string		path;

	// for sql
	int64_t			id;
};