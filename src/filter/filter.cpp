#include "filter.h"

using namespace std;

void set_last_items_to(deque<bool> &list, uint32_t nb_elts, bool value)
{
	uint32_t count = 0;
	auto it = list.end();
	for (; count < nb_elts; it--, count++)
		*it = value;
}

deque<bool> *filter_heat_data(const deque<float> &src_data,
		float threshold, uint32_t inertia_h_active,
		uint32_t inertia_h_inactive)
{
	deque<bool> *out_data = new deque<bool>();
	if (src_data.empty())
		return out_data;
	auto it = src_data.begin();
	auto itEnd = src_data.end();
	bool curr_state = *it >= threshold;
	for (; it != itEnd; it++) {
		out_data->push_back(curr_state);
		if (*it >= threshold) {
			if (!curr_state) {
				curr_state = true;
				set_last_items_to(*out_data, inertia_h_active
						+ 1, true);
			}
		} else {
			if (curr_state) {
				curr_state = false;
				set_last_items_to(*out_data, inertia_h_inactive
						+ 1, false);
			}
		}
	}
	return out_data;
}

/*
void write_values(bool* data, uint32_t size, uint32_t iStart, uint32_t iEnd,
		bool value)
{
	uint32_t i, iRealEnd = MIN(iEnd, size - 1);
	for (i = MAX(iStart, 0); i <= iRealEnd; i++) {
		data[i] = value;
	}
}

void filter_heat_data(float* src_data, bool* dst_data, uint32_t size,
		float threshold, uint32_t inertia_heater_active,
		uint32_t inertia_heater_inactive)
{
	if (size == 0)
		return;
	bool curr_state = src_data[0] >= threshold;
	uint32_t i;
	for (i = 0; i < size; i++) {
		dst_data[i] = curr_state;
		if (src_data[i] >= threshold) {
			if (!curr_state) {
				curr_state = true;
				write_values(dst_data, size,
						i - inertia_heater_active, i,
						true);
			}
		} else {
			if (curr_state) {
				curr_state = false;
				write_values(dst_data, size,
						i - inertia_heater_inactive, i,
						false);
			}
		}
	}
}
*/

