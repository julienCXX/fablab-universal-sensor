#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <string>
#include "filter.h"

using namespace std;

void usage(char *prog_name)
{
	cout << "Usage: " << prog_name << " THRESHOLD" << " INERT_HEAT_ACTIVE";
	cout << " INERT_HEAT_INACTIVE" << endl;
	cout << "Please feed data from standard input, output will be on standard output" << endl;
	cout << "\tTHRESHOLD\t\t" << "threshold temperature at which heater ";
	cout << "state change shall be detected" << endl;
	cout << "\tINERT_HEAT_ACTIVE\t";
	cout << "thermal inertia for heater activation detection" << endl;
	cout << "\t\t\t\t(how long the heater was on when temperature reached the threshold from below)" << endl;
	cout << "\tINERT_HEAT_INACTIVE\t";
	cout << "thermal inertia for heater deactivation detection" << endl;
	cout << "\t\t\t\t(how long the heater was off when temperature reached the threshold from above)" << endl;
}

int main(int argc, char** argv)
{
	if (argc < 4) {
		cerr << "Error: missing parameters" << endl;
		usage(argv[0]);
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
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	string line, substr_date, substr_temp;
	uint32_t idx_space;
	deque<string> dates;
	deque<float> raw_temp_values;
	deque<bool> *proc_values = NULL;
	cin.sync_with_stdio(false);
	while (!cin.eof()) {
		getline(cin, line);
		if (line.empty())
			continue;
		idx_space = line.find_first_of(' ');
		substr_date = line.substr(0, idx_space);
		substr_temp = line.substr(idx_space + 1);
		dates.push_back(substr_date);
		raw_temp_values.push_back(stof(substr_temp));
	}

	proc_values = filter_heat_data(raw_temp_values, threshold, in_h_active,
			in_h_inactive);
	if (proc_values == NULL) {
		cerr << "Error: not enough memory" << endl;
		return EXIT_FAILURE;
	}

	auto it_dates = dates.begin(), it_dates_end = dates.end();
	auto it_values = proc_values->begin();
	for (; it_dates != it_dates_end; it_dates++, it_values++)
		cout << *it_dates << ' ' << (*it_values ? 1 : 0) << endl;
	delete proc_values;
	return EXIT_SUCCESS;
}

