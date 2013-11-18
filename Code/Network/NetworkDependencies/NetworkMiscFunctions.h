#ifndef NET_MISC_FNC_H
#define NET_MISC_FNC_H
#include <string>
#include <vector>
#include <sstream>
std::vector<std::string> splitString(const char* p_inStr, char p_delim);
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
#endif