#pragma once

#include <string>
#include <sstream>

class Converter
{
public:
	template <typename T>
	static std::string  toString(T value)
	{
		std::ostringstream     st;

		st << value;
		return (st.str());
	}
	template <typename T>
	static T toInt(const std::string &str)
	{
		if (str.empty())
			return (0);
		std::istringstream	  st(str);
		T			  value;
		st >> value;
		return (value);
	}
};