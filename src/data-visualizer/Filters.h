#ifndef _H_FILTERS
#define _H_FILTERS

#include <cstdint>
#include <map>
#include "MeasureSet.h"

// filtering algorithm parameters
typedef struct {
	uint32_t heatMeasureIdx;
	union {
		float threshold; 
		struct {
			float thresholdHActive;
			float thresholdHInactive;
		};
	};
	uint32_t inertiaHActive;
	uint32_t inertiaHInactive;
	uint32_t minCountActive;
	uint32_t minCountInactive;
} ParamFilterHeatData;

typedef struct {
	uint32_t accMeasureXIdx;
	uint32_t accMeasureYIdx;
	uint32_t accMeasureZIdx;
	float minVarAcc;
	uint32_t minCountActive;
	uint32_t minCountInactive;
} ParamFilterAccelData;

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
				const ParamFilterHeatData &params);

		static void filterHeatDataAdvanced(MeasureSet &measures,
				const ParamFilterHeatData &params);

		static void filterAccelData(MeasureSet &measures,
				const ParamFilterAccelData &params);

};

#endif


