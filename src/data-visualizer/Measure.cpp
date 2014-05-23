#include "Measure.h"
#include <cassert>

using namespace std;

// see http://stackoverflow.com/questions/5167625/splitting-a-c-stdstring-using-tokens-e-g
vector<string> Measure::split(const string& s, char separator)
{
	vector<string> output;
	string::size_type prev_pos = 0, pos = 0;
	while ((pos = s.find(separator, pos)) != string::npos) {
		string substring(s.substr(prev_pos, pos-prev_pos));
		output.push_back(substring);
		prev_pos = ++pos;
	}
	output.push_back(s.substr(prev_pos, pos-prev_pos)); // final token
	return output;
}

Measure::Measure(const string &line, char separator,
				float result)
{
	vector<string> output = split(line, separator);
	assert(!output.empty());
	vector<string>::const_iterator it = output.begin();
	vector<string>::const_iterator itEnd = output.end();
	date = FormattedDate(*it);
	it++;
	while (it != itEnd) {
		values.push_back(stof(it->c_str()));
		it++;
	}
	this->result = result;
}

const FormattedDate &Measure::getDate() const
{
	return date;
}

const std::vector<float> &Measure::getValues() const
{
	return values;
}

int Measure::nbOfValues() const
{
	return values.size();
}

bool Measure::getBoolResult() const
{
	return result != 0.0;
}

void Measure::setResult(bool value)
{
	result = value ? 1.0 : 0.0;
}

float Measure::getFloatResult() const
{
	return result;
}

void Measure::setResult(float value)
{
	result = value;
}

bool Measure::operator==(const Measure &other) const
{
	return date == other.date;
}

bool Measure::operator!=(const Measure &other) const
{
	return date != other.date;
}

bool Measure::operator>(const Measure &other) const
{
	return date > other.date;
}

bool Measure::operator<(const Measure &other) const
{
	return date < other.date;
}

bool Measure::operator>=(const Measure &other) const
{
	return date >= other.date;
}

bool Measure::operator<=(const Measure &other) const
{
	return date <= other.date;
}

float Measure::operator[](int index) const
{
	return values[index];
}

