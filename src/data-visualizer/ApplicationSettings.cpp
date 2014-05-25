#include "ApplicationSettings.h"

using namespace std;

ApplicationSettings::ApplicationSettings(const string &configFile)
{
	conf.readFile(configFile.c_str());
}

string ApplicationSettings::getInputFileName() const
{
	string s;
	if (conf.lookupValue("fileName", s))
		return s;
	return "";
}

string ApplicationSettings::getInputFilePath() const
{
	string s;
	if (conf.lookupValue("filePath", s))
		return s;
	return "measures/";
}

FilterType ApplicationSettings::getFilterType() const
{
	string s;
	if (conf.lookupValue("filterType", s)) {
		if (s == "ON_OFF_HEAT")
			return ON_OFF_HEAT;
		if (s == "ON_OFF_HEAT_ADVANCED")
			return ON_OFF_HEAT_ADVANCED;
		if (s == "ON_OFF_ACCELERATION")
			return ON_OFF_ACCELERATION;
		if (s == "WATER_FLOW_ACCELERATION")
			return WATER_FLOW_ACCELERATION;
	}
	return NONE;
}

ParamFilterData ApplicationSettings::getFilterParameters() const
{
	ParamFilterData fd;
	fd.threshold = 28.5;
	fd.thresholdHActive = 26.0;
	fd.thresholdHInactive = 28.4;
	fd.inertiaHActive = 3;
	fd.inertiaHInactive = 7;
	fd.heatMeasureIdx = 0;
	fd.minVarAcc = 0.2;
	fd.accMeasureXIdx = 0;
	fd.accMeasureYIdx = 1;
	fd.accMeasureZIdx = 2;
	fd.minCountActive = 2;
	fd.minCountInactive = 4;
	fd.flowFactor = 15.3;
	conf.lookupValue("filterConfiguration.threshold", fd.threshold);
	conf.lookupValue("filterConfiguration.thresholdHActive", fd.thresholdHActive);
	conf.lookupValue("filterConfiguration.thresholdHInactive", fd.thresholdHInactive);
	conf.lookupValue("filterConfiguration.inertiaHActive", fd.inertiaHActive);
	conf.lookupValue("filterConfiguration.inertiaHInactive", fd.inertiaHInactive);
	conf.lookupValue("filterConfiguration.heatMeasureIdx", fd.heatMeasureIdx);
	conf.lookupValue("filterConfiguration.minVarAcc", fd.minVarAcc);
	conf.lookupValue("filterConfiguration.xIndex", fd.accMeasureXIdx);
	conf.lookupValue("filterConfiguration.yIndex", fd.accMeasureYIdx);
	conf.lookupValue("filterConfiguration.zIndex", fd.accMeasureZIdx);
	conf.lookupValue("filterConfiguration.minCountActive", fd.minCountActive);
	conf.lookupValue("filterConfiguration.minCountInactive", fd.minCountInactive);
	conf.lookupValue("filterConfiguration.flowFactor", fd.flowFactor);

	return fd;
}

JsonMeasureFormat ApplicationSettings::getOutputConfig() const
{
	bool b;
	int i;
	JsonMeasureFormat jmf;
	jmf.setIsBoolResult(true);
	jmf.setValueField(0, "x");
	jmf.setValueField(1, "y");
	jmf.setValueField(2, "z");
	if (conf.lookupValue("outputConfiguration.isBoolResult", b))
		jmf.setIsBoolResult(b);
	if (conf.lookupValue("outputConfiguration.irTempIdx", i))
		jmf.setValueField(i, "irTemp");
	if (conf.lookupValue("outputConfiguration.ambTempIdx", i))
		jmf.setValueField(i, "ambTemp");
	if (conf.lookupValue("outputConfiguration.pressureIdx", i))
		jmf.setValueField(i, "pressure");
	if (conf.lookupValue("outputConfiguration.humidityIdx", i))
		jmf.setValueField(i, "humidity");
	if (conf.lookupValue("outputConfiguration.accXIdx", i))
		jmf.setValueField(i, "accX");
	if (conf.lookupValue("outputConfiguration.accYIdx", i))
		jmf.setValueField(i, "accY");
	if (conf.lookupValue("outputConfiguration.accZIdx", i))
		jmf.setValueField(i, "accZ");
	if (conf.lookupValue("outputConfiguration.magnXIdx", i))
		jmf.setValueField(i, "magnX");
	if (conf.lookupValue("outputConfiguration.magnYIdx", i))
		jmf.setValueField(i, "magnY");
	if (conf.lookupValue("outputConfiguration.magnZIdx", i))
		jmf.setValueField(i, "magnZ");
	if (conf.lookupValue("outputConfiguration.gyrXIdx", i))
		jmf.setValueField(i, "gyrX");
	if (conf.lookupValue("outputConfiguration.gyrYIdx", i))
		jmf.setValueField(i, "gyrY");
	if (conf.lookupValue("outputConfiguration.gyrZIdx", i))
		jmf.setValueField(i, "gyrZ");

	return jmf;
}

bool ApplicationSettings::getShowDailyStatistics() const
{
	bool b;
	if (conf.lookupValue("outputConfiguration.showDailyStatistics", b))
		return b;
	return false;
}

