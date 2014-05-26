#ifndef _H_FILTERS
#define _H_FILTERS

#include <cstdint>
#include <map>
#include "MeasureSet.h"

// filtering algorithm parameters
typedef struct {
	// parameters for heat-based filters
	uint32_t inertiaHActive;
	uint32_t inertiaHInactive;
	uint32_t heatMeasureIdx;
	// basic filter
	float threshold;
	// advanced filter
	float thresholdHActive;
	float thresholdHInactive;
	// parameters for acceleration-based filters
	uint32_t accMeasureXIdx;
	uint32_t accMeasureYIdx;
	uint32_t accMeasureZIdx;
	float minVarAcc;
	// parameter for water flow measurement
	float flowFactor;
	// parameters for both heat and acceleration-based filtering
	uint32_t minCountActive;
	uint32_t minCountInactive;
} ParamFilterData;

// the filtering algorithms, organized into a class
class Filters
{
	private:
		static void setLastItemsTo(MeasureSet &measures,
				const std::map<FormattedDate, Measure>::iterator
				&curEnd, uint32_t nb_elts, bool value);
		Filters();

	public:
		static void filterHeatData(MeasureSet &measures,
				const ParamFilterData &params);

		static void filterHeatDataAdvanced(MeasureSet &measures,
				const ParamFilterData &params);

		static void filterAccelData(MeasureSet &measures,
				const ParamFilterData &params);

};

#endif


