#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <deque>
#include <string>
#include "filter.h"
#include "dateconv.h"

using namespace std;

// see http://stackoverflow.com/questions/5167625/splitting-a-c-stdstring-using-tokens-e-g
vector<string> split(const string& s, char separator)
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

void line_to_float(const string &line, time_t *date, float *value)
{
	vector<string> tokens = split(line, ' ');
	const char *s_val = tokens[1].c_str();
	*value = stof(s_val);
	*date = t_string_to_time(tokens[0]);
}

void line_to_accel_tuple(const string &line, time_t *date,
		tuple<float, float, float> *tuple)
{
	vector<string> tokens = split(line, ' ');
	const char *s_x = tokens[1].c_str(),
	     *s_y = tokens[2].c_str(),
	     *s_z = tokens[3].c_str();
	const float x = stof(s_x),
	      y = stof(s_y),
	      z = stof(s_z);
	*date = t_string_to_time(tokens[0]);
	*tuple = make_tuple(x, y, z);
}

string bool_to_json_heat_on_off(const bool val)
{
	if (val)
		return "\"hOff\":0,\"hOn\":1";
	else
		return "\"hOff\":1,\"hOn\":0";
}

void print_temp_chart_element(ostream &stream, time_t date, bool value,
		float temp)
{
	stream << '{';
	stream << "\"temp\":" << temp << ',';
	stream << "\"date\":\"" << time_to_string(date) << "\",";
	stream << bool_to_json_heat_on_off(value) << '}' << endl;
}

void print_acc_chart_element(ostream &stream, time_t date, bool value,
		const tuple<float, float, float> &acc)
{
	stream << '{';
	stream << "\"x\":" << get<0>(acc) + 2 << ',';
	stream << "\"y\":" << get<1>(acc) << ',';
	stream << "\"z\":" << get<2>(acc) - 2 << ',';
	stream << "\"date\":\"" << time_to_string(date) << "\",";
	stream << bool_to_json_heat_on_off(value) << '}' << endl;
}

/*
void usage(char *prog_name)
{
}
*/

int main(int argc, char** argv)
{
	if (argc < 4) {
		cerr << "Error: missing parameters" << endl;
		//usage(argv[0]);
		return EXIT_FAILURE;
	}

	float threshold;
	uint32_t in_h_active, in_h_inactive;
	try {
		threshold = stof(argv[1]);
		in_h_active = stoul(argv[2]);
		in_h_inactive = stoul(argv[3]);
	} catch (exception e) {
		cerr << "Error: wrong parameter type" << endl;
		//usage(argv[0]);
		return EXIT_FAILURE;
	}
	ofstream jsdat;
	jsdat.open("jsdat.js");
	string line;
	deque<time_t> dates;
	time_t date;
	float raw_temp;
	deque<float> raw_temps;
	deque<tuple<float, float, float>> raw_accs;
	tuple<float, float, float> raw_acc;
	deque<bool> *proc_values = NULL;
	//cin.sync_with_stdio(false);
	while (!cin.eof()) {
		getline(cin, line);
		if (line.empty())
			continue;
		line_to_float(line, &date, &raw_temp);
		//line_to_accel_tuple(line, &date, &raw_acc);
		dates.push_back(date);
		raw_temps.push_back(raw_temp);
		//raw_accs.push_back(raw_acc);
	}

	/*proc_values = filter_accel_data(raw_accs, threshold, in_h_active,
			in_h_inactive);*/
	proc_values = filter_heat_data(raw_temps, threshold, in_h_active,
			in_h_inactive);
	if (proc_values == NULL) {
		cerr << "Error: not enough memory" << endl;
		return EXIT_FAILURE;
	}

	auto it_dates = dates.begin(), it_dates_end = dates.end();
	auto it_values = proc_values->begin();
	//auto it_raw = raw_accs.begin();
	auto it_raw = raw_temps.begin();
	jsdat << "var chartData = [";
	print_temp_chart_element(jsdat, *it_dates, *it_values, *it_raw);
	//print_acc_chart_element(jsdat, *it_dates, *it_values, *it_raw);
	it_dates++;
	it_values++;
	it_raw++;
	for (; it_dates != it_dates_end; it_dates++, it_values++, it_raw++) {
		jsdat << ',';
		print_temp_chart_element(jsdat, *it_dates, *it_values, *it_raw);
		//print_acc_chart_element(jsdat, *it_dates, *it_values, *it_raw);
	}
	jsdat << "];";
	delete proc_values;
	jsdat.close();
	return EXIT_SUCCESS;
}

