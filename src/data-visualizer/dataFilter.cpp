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
#include "FormattedDate.h"
#include "Filters.h"

using namespace std;

int main(int argc, char **argv)
{
	ApplicationSettings as;
	if (argc > 1)
		as.readFromFile(string(argv[1]));
	cerr << "file read" << endl;
	string fileName = as.getInputFileName(),
	       filePath = as.getInputFilePath();
	fs::path path(fs::current_path());
	MeasureSet ms;
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
	
	// writing results to output
	cerr << "Saving results" << endl;
	ofstream jsdat;
	jsdat.open("jsdat.js");
	jsdat << "var chartData = " << ms.toJsonFormat(as.getOutputConfig())
		<< endl;
	jsdat.close();
	return EXIT_SUCCESS;
}

