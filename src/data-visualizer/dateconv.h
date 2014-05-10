
#ifndef _H_DATECONV
#define _H_DATECONV

#include <string>
#include <ctime>

time_t t_string_to_time(const std::string &string);

std::string time_to_string(const time_t time);

#endif

