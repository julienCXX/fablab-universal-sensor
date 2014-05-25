#ifndef _H_APPLICATION_SETTINGS
#define _H_APPLICATION_SETTINGS

#include "Filters.h"
#include "JsonMeasureFormat.h"
#include <libconfig.h++>

typedef enum {
	HEAT,
	HEAT_ADVANCED,
	ACCELERATION
} FilterType;

// the program's global settings
class ApplicationSettings
{
	private:
		libconfig::Config conf;

	public:
		ApplicationSettings(const std::string &configFile = "");

		std::string getInputFileName() const;
		std::string getInputFilePath() const;
		FilterType getFilterType() const;
		ParamFilterData getFilterParameters() const;
		JsonMeasureFormat getOutputConfig() const;
		bool getShowDailyStatistics() const;

};

#endif

