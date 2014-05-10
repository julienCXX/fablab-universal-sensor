#include "dateconv.h"

#include <cmath>

using namespace std;

time_t t_string_to_time(const string &string)
{
	static const char* FORMAT = "%Y-%m-%dT%T";
	struct tm time_struct;
	strptime(string.c_str(), FORMAT, &time_struct);
	return mktime(&time_struct);
}

string time_to_string(const time_t time)
{
	static const char* FORMAT = "%Y-%m-%d %T";
	static char buffer[32];
	struct tm time_struct;
	localtime_r(&time, &time_struct);
	strftime(buffer, 32, FORMAT, &time_struct);
	return string(buffer);
}

