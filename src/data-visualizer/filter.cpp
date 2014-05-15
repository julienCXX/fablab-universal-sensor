#include "filter.h"

#include <cmath>

using namespace std;

void set_last_items_to(deque<bool> &list, uint32_t nb_elts, bool value)
{
	uint32_t count = 0;
	deque<bool>::reverse_iterator it = list.rbegin();
	deque<bool>::reverse_iterator it_end = list.rend();
	for (; count < nb_elts && it != it_end; it++, count++)
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
	auto it_end = src_data.end();
	bool curr_state = *it >= threshold;
	for (; it != it_end; it++) {
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

deque<bool> *filter_heat_data(const deque<float> &src_data,
		float threshold_h_active, float threshold_h_inactive,
		uint32_t inertia_h_active, int32_t inertia_h_inactive,
		uint32_t min_count_active, uint32_t min_count_inactive)
{
	deque<bool> *out_data = new deque<bool>();
	if (src_data.empty())
		return out_data;
	auto it = src_data.begin();
	auto it_end = src_data.end();
	bool curr_state = false;
	uint32_t curr_count = 0;
	for (; it != it_end; it++) {
		out_data->push_back(curr_state);
		if (curr_state) {
			if (*it < threshold_h_inactive) {
				if (curr_count >= min_count_inactive) {
					curr_count = 0;
					curr_state = false;
					set_last_items_to(*out_data,
							min_count_inactive
							+ inertia_h_inactive
							+ 1, false);
				} else {
					curr_count++;
				}
			} else {
				curr_count = 0;
			}
		} else {
			if (*it >= threshold_h_active) {
				if (curr_count >= min_count_active) {
					curr_count = 0;
					curr_state = true;
					set_last_items_to(*out_data,
							min_count_active
							+ inertia_h_active
							+ 1, true);
				} else {
					curr_count++;
				}
			} else {
				curr_count = 0;
			}
		}
	}
	return out_data;
}

deque<bool> *filter_accel_data(const deque<tuple<float, float, float>> &xyz_acc,
		float min_var_acc, uint32_t min_count_active,
		uint32_t min_count_inactive)
{
	deque<bool> *out_data = new deque<bool>();
	if (xyz_acc.empty())
		return out_data;
	auto it = xyz_acc.begin();
	auto it_end = xyz_acc.end();
	bool curr_state = false;
	float prev_x = 0, prev_y = 0, prev_z = 0, delta_x, delta_y, delta_z,
	      temp;
	uint32_t inert_count = 0;
	for (; it != it_end; it++) {
		out_data->push_back(curr_state);
		// computing deltas
		temp = get<0>(*it);
		delta_x = abs(temp - prev_x);
		prev_x = temp;
		temp = get<1>(*it);
		delta_y = abs(temp - prev_y);
		prev_y = temp;
		temp = get<2>(*it);
		delta_z = abs(temp - prev_z);
		prev_z = temp;

		if (delta_x >= min_var_acc || delta_y >= min_var_acc
				|| delta_z >= min_var_acc) {
			// vibration detected
			if (curr_state) {
				inert_count = 0;
			} else {
				inert_count++;
				if (inert_count >= min_count_active) {
					/*
					 * enough vibrations in a row, count
					 * them as part of "active heater" mode
					 */
					inert_count = 0;
					curr_state = true;
					set_last_items_to(*out_data,
							min_count_active + 1,
							true);
				}
			}
		} else {
			if (!curr_state) {
				inert_count = 0;
			} else {
				inert_count++;
				if (inert_count >= min_count_inactive) {
					inert_count = 0;
					curr_state = false;
					set_last_items_to(*out_data,
							min_count_inactive + 1,
							false);
				}
			}
		}
	}
	return out_data;
}

