#ifndef BOOST_FILESYSTEM_NO_DEPRECATED 
#  define BOOST_FILESYSTEM_NO_DEPRECATED
#endif
#ifndef BOOST_SYSTEM_NO_DEPRECATED 
#  define BOOST_SYSTEM_NO_DEPRECATED
#endif

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <iostream>
#include <fstream>
#include <cassert>

namespace fs = boost::filesystem;

#include "ApplicationSettings.h"
#include "MeasureSet.h"
#include "Measure.h"
#include "FormattedDate.h"
#include "Filters.h"

using namespace std;

string measureAsFactualToJsonFormat(const Measure &mes)
{
	string output;
	output.append("\"date\":\"" + mes.getDate().toString() + "\"");
	output.append(",\"above\":true,\"inside\":true");
	output.append(",\"lineAlpha\":1,\"labelRotation\":90");
	if (mes[0] == 0.0)
		output.append(",\"label\":\"Factual heater stop\"");
	else
		output.append(",\"label\":\"Factual heater start\"");
	return output;
}

string iterateOnMeasuresAsFactualToJsonFormat(const MeasureSet &ms)
{
	auto it = ms.cbegin(), itEnd = ms.cend();
	string output = "var guides = [";
	if (it != itEnd) {
		output.append("{");
		output.append(measureAsFactualToJsonFormat(it->second) + "}");
		it++;
	}
	while (it != itEnd) {
		output.append(",{");
		output.append(measureAsFactualToJsonFormat(it->second) + "}");
		it++;
	}
	output.append("]");
	return output;
}

string measureAsDayStatToJsonFormat(const Measure &mes)
{
	string output;
	output.append("\"date\":\"" + mes.getDate().toStringDayOnly() + "\"");
	output.append(",\"dActiv\":" + to_string((long int) mes.getFloatResult()));
	return output;
}

string iterateOnMeasuresAsDayStatToJsonFormat(const MeasureSet &ms)
{
	auto it = ms.cbegin(), itEnd = ms.cend();
	string output = "var dayStat = [";
	if (it != itEnd) {
		output.append("{");
		output.append(measureAsDayStatToJsonFormat(it->second) + "}");
		it++;
	}
	while (it != itEnd) {
		output.append(",{");
		output.append(measureAsDayStatToJsonFormat(it->second) + "}");
		it++;
	}
	output.append("]");
	return output;
}

void computeDStats(const MeasureSet &ms, MeasureSet &dayStatMS)
{
	auto it = ms.cbegin(), itEnd = ms.cend();
	FormattedDate curDate, nextDate;
	float nbSecs = 0.0;
	if (it == itEnd)
		return;
	curDate = it->second.getDate();
	it++;
	while (it != itEnd) {
		nextDate = it->second.getDate();
		if (curDate.sameDayAs(nextDate)) {
			nbSecs += it->second.getFloatResult();
		} else {
			dayStatMS.add(Measure(curDate, nbSecs));
			nbSecs = 0.0;
		}
		curDate = nextDate;
		it++;
	}
	dayStatMS.add(Measure(curDate, nbSecs));
}

int main(int argc, char **argv)
{
	ApplicationSettings as;
	if (argc > 1)
		as.readFromFile(string(argv[1]));
	cerr << "file read" << endl;
	string fileName = as.getInputFileName(),
	       filePath = as.getInputFilePath();
	fs::path path(fs::current_path());
	MeasureSet ms, factualMS, dayStatMS;
	assert(!fileName.empty() || !filePath.empty());

	// loading raw data files
	if (!fileName.empty()) {
		ms.add(fileName);
	} else {
		// add all the files in the current path
		// (subdirectories excluded)
		cerr << "Reading measures from path: " << filePath << endl;
		path = fs::system_complete(filePath);
		assert(fs::is_directory(path));
		fs::directory_iterator endIter;
		for (fs::directory_iterator dirItr(path); dirItr != endIter;
				++dirItr) {
			if (!fs::is_regular_file(dirItr->status()))
				continue;
			cerr << "Adding " << dirItr->path().string()
				<< " contents" << endl;
			ms.add(dirItr->path().string());
		}
	}
	// loading factual measures, if existing
	if (!as.getFactualStateChangeFileName().empty())
		factualMS.add(as.getFactualStateChangeFileName());

	// applying filter
	cerr << "Filtering" << endl;
	switch (as.getFilterType()) {
		case ON_OFF_HEAT:
			Filters::filterHeatData(ms, as.getFilterParameters());
			break;
		case ON_OFF_HEAT_ADVANCED:
			Filters::filterHeatDataAdvanced(ms,
					as.getFilterParameters());
			break;
		case ON_OFF_ACCELERATION:
			Filters::filterAccelData(ms, as.getFilterParameters());
			break;
		case WATER_FLOW_ACCELERATION:
			break;
		case NONE:
			break;
	}
	
	// computing daily statistics
	computeDStats(ms, dayStatMS);

	// writing results to output
	cerr << "Saving results" << endl;
	ofstream jsdat;
	jsdat.open("jsdat.js");
	jsdat << "var chartData = " << ms.toJsonFormat(as.getOutputConfig())
		<< endl;
	jsdat << iterateOnMeasuresAsFactualToJsonFormat(factualMS) << endl;
	jsdat << iterateOnMeasuresAsDayStatToJsonFormat(dayStatMS) << endl;
	jsdat.close();
	return EXIT_SUCCESS;
}

