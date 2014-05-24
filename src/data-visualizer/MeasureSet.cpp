#include "MeasureSet.h"
#include <fstream>

using namespace std;

typedef map<FormattedDate, Measure>::iterator MSIt;
typedef map<FormattedDate, Measure>::const_iterator MSCit;
typedef map<FormattedDate, Measure>::reverse_iterator MSRit;

void MeasureSet::add(const Measure &measure)
{
	measures.insert(pair<FormattedDate, Measure>(measure.getDate(), measure));
}

void MeasureSet::add(const string &filePath, const char separ)
{
	ifstream file;
	string line;
	file.open(filePath);
	while (!file.eof()) {
		getline(file, line);
		if (line.empty() || line[0] == '#')
			continue;
		add(Measure(line, separ));
	}
	file.close();
}

MSIt MeasureSet::begin()
{
	return measures.begin();
}

MSIt MeasureSet::end()
{
	return measures.end();
}

MSCit MeasureSet::cbegin() const
{
	return measures.cbegin();
}

MSCit MeasureSet::cend() const
{
	return measures.cend();
}

MSRit MeasureSet::rbegin()
{
	return measures.rbegin();
}

MSRit MeasureSet::rend()
{
	return measures.rend();
}

string MeasureSet::toJsonFormat(const JsonMeasureFormat &format) const
{
	string output;
	output.append("[");
	auto it = cbegin(), itEnd = cend();
	if (it != itEnd) {
		output.append("{" + it->second.toJsonFormat(format) + "}");
		it++;
	}
	while (it != itEnd) {
		output.append(",{");
		output.append(it->second.toJsonFormat(format) + "}");
	}
	output.append("]");

	return output;
}

