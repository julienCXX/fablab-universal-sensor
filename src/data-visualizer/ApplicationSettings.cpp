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

FilterType ApplicationSettings::getFilterType() const
{
	return ACCELERATION;
}

ParamFilterData ApplicationSettings::getFilterParameters() const
{
	ParamFilterData fd;
	fd.accMeasureXIdx = 0;
	fd.accMeasureYIdx = 1;
	fd.accMeasureZIdx = 2;
	fd.minVarAcc = 0.2;
	fd.minCountActive = 2;//3;
	fd.minCountInactive = 4;//5;
	return fd;
}

JsonMeasureFormat ApplicationSettings::getOutputConfig() const
{
	JsonMeasureFormat jmf;
	jmf.setIsBoolResult(true);
	jmf.setValueField(0, "x");
	jmf.setValueField(1, "y");
	jmf.setValueField(2, "z");

	return jmf;
}

bool ApplicationSettings::getShowDailyStatistics() const
{
	return false;
}

