#include "ApplicationSettings.h"

using namespace std;

ApplicationSettings::ApplicationSettings(const string &configFile)
{
}

string ApplicationSettings::getInputFileName() const
{
	return "";
}

string ApplicationSettings::getInputFilePath() const
{
	return "measures/";
}

ParamFilterData ApplicationSettings::getFilterParameters() const
{
	ParamFilterData fd;
	return fd;
}

JsonMeasureFormat ApplicationSettings::getOutputConfig() const
{
	JsonMeasureFormat jmf;
	return jmf;
}

bool ApplicationSettings::getShowDailyStatistics() const
{
	return false;
}

