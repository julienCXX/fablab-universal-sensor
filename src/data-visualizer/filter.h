
#ifndef _H_FILTER
#define _H_FILTER

#include <deque>
#include <tuple>
#include <cstdint>

std::deque<bool> *filter_heat_data(const std::deque<float> &src_data,
		float threshold, uint32_t inertia_h_active,
		uint32_t inertia_h_inactive);

std::deque<bool> *filter_heat_data(const std::deque<float> &src_data,
		float threshold_h_active, float threshold_h_inactive,
		uint32_t inertia_h_active, int32_t inertia_h_inactive,
		uint32_t min_count_active, uint32_t min_count_inactive);

std::deque<bool> *filter_accel_data(const std::deque<std::tuple<float, float, float>> &xyz_acc,
		float min_var_acc, uint32_t min_count_active,
		uint32_t min_count_inactive);

#endif

