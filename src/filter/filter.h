
#ifndef _H_FILTER
#define _H_FILTER

#include <deque>
#include <cstdint>

std::deque<bool> *filter_heat_data(const std::deque<float> &src_data,
		float threshold, uint32_t inertia_h_active,
		uint32_t inertia_h_inactive);

#endif

