#include "NetworkMiscFunctions.h"
std::vector<std::string> splitString(const char* p_inStr, char p_delim)
{
	std::stringstream ss(p_inStr);
	std::vector<std::string> elems;
	std::string item;
	while(std::getline(ss, item, p_delim)) 
	{
		elems.push_back(item);
	}
	return elems;
}