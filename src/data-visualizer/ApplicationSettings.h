#ifndef _H_APPLICATION_SETTINGS
#define _H_APPLICATION_SETTINGS

#include "Filters.h"
#include "JsonMeasureFormat.h"
#include <libconfig.h++>

typedef enum {
	NONE,
	ON_OFF_HEAT,
	ON_OFF_HEAT_ADVANCED,
	ON_OFF_ACCELERATION,
	WATER_FLOW_ACCELERATION
} FilterType;

// the program's global settings
class ApplicationSettings
{
	private:
		libconfig::Config conf;

	public:
		ApplicationSettings(const std::string &configFile = "settings.conf");

		void readFromFile(const std::string &configFile);
		std::string getInputFileName() const;
		std::string getInputFilePath() const;
		FilterType getFilterType() const;
		ParamFilterData getFilterParameters() const;
		JsonMeasureFormat getOutputConfig() const;
		bool getShowDailyStatistics() const;

};

#endif

