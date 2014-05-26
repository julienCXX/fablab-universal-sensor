#include "Filters.h"
#include "FormattedDate.h"
#include <algorithm>
#include <cmath>

using namespace std;

void Filters::setLastItemsTo(MeasureSet &measures,
		const map<FormattedDate, Measure>::iterator &curEnd,
		uint32_t nbElts, bool value)
{
	uint32_t count = 0;
	auto orgEnd = curEnd;
	orgEnd++;
	map<FormattedDate, Measure>::reverse_iterator it(orgEnd),
		itEnd = measures.rend();
	for (; count < nbElts && it != itEnd; it++, count++)
		it->second.setResult(value);
}

Filters::Filters()
{
}

void Filters::filterHeatData(MeasureSet &measures,
		const ParamFilterData &params)
{
	auto it = measures.begin();
	auto itEnd = measures.end();
	if (it == itEnd)
		return;
	bool currState = it->second[params.heatMeasureIdx] >= params.threshold;
	for (; it != itEnd; it++) {
		it->second.setResult(currState);
		if (it->second[params.heatMeasureIdx] >= params.threshold) {
			if (!currState) {
				currState = true;
				setLastItemsTo(measures, it,
						params.inertiaHActive + 1,
						true);
			}
		} else {
			if (currState) {
				currState = false;
				setLastItemsTo(measures, it,
						params.inertiaHInactive + 1,
						false);
			}
		}
	}
}

void Filters::filterHeatDataAdvanced(MeasureSet &measures,
		const ParamFilterData &params)
{
	auto it = measures.begin();
	auto itEnd = measures.end();
	if (it == itEnd)
		return;
	bool currState = false;
	uint32_t currCount = 0;
	for (; it != itEnd; it++) {
		it->second.setResult(currState);
		if (currState) {
			if (it->second[params.heatMeasureIdx] < params.thresholdHInactive) {
				if (currCount >= params.minCountInactive) {
					currCount = 0;
					currState = false;
					setLastItemsTo(measures, it,
							params.minCountInactive
							+ params.inertiaHInactive
							+ 1, false);
				} else {
					currCount++;
				}
			} else {
				currCount = 0;
			}
		} else {
			if (it->second[params.heatMeasureIdx] >= params.thresholdHActive) {
				if (currCount >= params.minCountActive) {
					currCount = 0;
					currState = true;
					setLastItemsTo(measures, it,
							params.minCountActive
							+ params.inertiaHActive
							+ 1, true);
				} else {
					currCount++;
				}
			} else {
				currCount = 0;
			}
		}
	}
}

void Filters::filterAccelData(MeasureSet &measures,
		const ParamFilterData &params)
{
	auto it = measures.begin();
	auto itEnd = measures.end();
	if (it == itEnd)
		return;
	bool currState = false;
	float prevX = 0, prevY = 0, prevZ = 0, deltaX, deltaY, deltaZ, temp;
	uint32_t inertCount = 0;
	for (; it != itEnd; it++) {
		it->second.setResult(currState);
		// computing deltas
		temp = it->second[params.accMeasureXIdx];
		deltaX = abs(temp - prevX);
		prevX = temp;
		temp = it->second[params.accMeasureYIdx];
		deltaY = abs(temp - prevY);
		prevY = temp;
		temp = it->second[params.accMeasureZIdx];
		deltaZ = abs(temp - prevZ);
		prevZ = temp;

		if (deltaX >= params.minVarAcc || deltaY >= params.minVarAcc
				|| deltaZ >= params.minVarAcc) {
			// vibration detected
			if (currState) {
				inertCount = 0;
			} else {
				inertCount++;
				if (inertCount >= params.minCountActive) {
					/*
					 * enough vibrations in a row, count
					 * them as part of "active heater" mode
					 */
					inertCount = 0;
					currState = true;
					setLastItemsTo(measures, it,
							params.minCountActive
							+ 1, true);
				}
			}
		} else {
			if (!currState) {
				inertCount = 0;
			} else {
				inertCount++;
				if (inertCount >= params.minCountInactive) {
					inertCount = 0;
					currState = false;
					setLastItemsTo(measures, it,
							params.minCountInactive
							+ 1, false);
				}
			}
		}
	}
}


void Filters::computeWaterFlow(MeasureSet &measures,
		const ParamFilterData &params)
{
	auto it = measures.begin();
	auto itEnd = measures.end();
	if (it == itEnd)
		return;
	bool currState = false;
	float prevX = 0, prevY = 0, prevZ = 0, deltaX, deltaY, deltaZ, temp;
	FormattedDate prevDate = it->first;
	float inertCount = 0.0, timeDiff, avgVar;
	for (; it != itEnd; it++) {
		timeDiff = it->first.absSecondsDiff(prevDate);
		// computing deltas
		temp = it->second[params.accMeasureXIdx];
		deltaX = abs(temp - prevX);
		prevX = temp;
		temp = it->second[params.accMeasureYIdx];
		deltaY = abs(temp - prevY);
		prevY = temp;
		temp = it->second[params.accMeasureZIdx];
		deltaZ = abs(temp - prevZ);
		prevZ = temp;

		// computing flow value
		if (currState) {
			avgVar = (deltaX + deltaY + deltaZ) / 3.0;
			it->second.setResult(avgVar * params.flowFactor * timeDiff);
		} else {
			it->second.setResult(0.0f);
		}

		if (deltaX >= params.minVarAcc || deltaY >= params.minVarAcc
				|| deltaZ >= params.minVarAcc) {
			// vibration detected
			if (currState) {
				inertCount = 0.0;
			} else {
				inertCount += timeDiff;
				if (inertCount >= params.minCountActive) {
					/*
					 * enough vibrations in a row, count
					 * start counting as water flow
					 */
					inertCount = 0.0;
					currState = true;
				}
			}
		} else {
			if (!currState) {
				inertCount = 0.0;
			} else {
				inertCount += timeDiff;
				if (inertCount >= params.minCountInactive) {
					inertCount = 0.0;
					currState = false;
				}
			}
		}
	}
}

float Filters::findMaxVariation(const MeasureSet &ms)
{
	auto it = ms.cbegin();
	auto itEnd = ms.cend();
	if (it == itEnd)
		return 0.0;
	float prevX = 0, prevY = 0, prevZ = 0, deltaX, deltaY, deltaZ, temp;
	float maxVar = 0.0, timeDiff;
	FormattedDate prevDate = it->first;
	for (; it != itEnd; it++) {
		timeDiff = it->first.absSecondsDiff(prevDate);
		// computing deltas
		temp = it->second[0];
		deltaX = abs(temp - prevX);
		prevX = temp;
		temp = it->second[1];
		deltaY = abs(temp - prevY);
		prevY = temp;
		temp = it->second[2];
		deltaZ = abs(temp - prevZ);
		prevZ = temp;

		// setting new max (measure duration aware))
		if (deltaX * timeDiff > maxVar)
			maxVar = deltaX * timeDiff;
		if (deltaY * timeDiff > maxVar)
			maxVar = deltaY * timeDiff;
		if (deltaZ * timeDiff > maxVar)
			maxVar = deltaZ * timeDiff;
	}
	return maxVar;
}

float Filters::findAvgVariation(const MeasureSet &ms)
{
	auto it = ms.cbegin();
	auto itEnd = ms.cend();
	if (it == itEnd)
		return 0.0;
	float prevX = 0, prevY = 0, prevZ = 0, deltaX, deltaY, deltaZ, temp;
	float sumVar = 0.0, timeDiff;
	float varCount = 0;
	FormattedDate prevDate = it->first;
	for (; it != itEnd; it++) {
		timeDiff = it->first.absSecondsDiff(prevDate);
		// computing deltas
		temp = it->second[0];
		deltaX = abs(temp - prevX);
		prevX = temp;
		temp = it->second[1];
		deltaY = abs(temp - prevY);
		prevY = temp;
		temp = it->second[2];
		deltaZ = abs(temp - prevZ);
		prevZ = temp;

		// adding new varations
		sumVar += deltaX * timeDiff;
		sumVar += deltaY * timeDiff;
		sumVar += deltaZ * timeDiff;
		varCount += 3 * timeDiff;
	}
	return sumVar / varCount;
}

float Filters::findMaxNoiseLength(const MeasureSet &ms, float minVar)
{
	auto it = ms.cbegin();
	auto itEnd = ms.cend();
	if (it == itEnd)
		return 0.0;
	float prevX = 0, prevY = 0, prevZ = 0, deltaX, deltaY, deltaZ, temp;
	float curLen = 0.0, maxLen = 0.0, timeDiff;
	FormattedDate prevDate = it->first;
	for (; it != itEnd; it++) {
		timeDiff = it->first.absSecondsDiff(prevDate);
		// computing deltas
		temp = it->second[0];
		deltaX = abs(temp - prevX);
		prevX = temp;
		temp = it->second[1];
		deltaY = abs(temp - prevY);
		prevY = temp;
		temp = it->second[2];
		deltaZ = abs(temp - prevZ);
		prevZ = temp;

		if (deltaX > minVar * timeDiff || deltaY > minVar * timeDiff
				|| deltaZ > minVar * timeDiff) {
			curLen += timeDiff;
			if (curLen > maxLen)
				maxLen = curLen;
		} else {
			curLen = 0.0;
		}
	}
	return maxLen;
}

float Filters::findFlowFactor(const MeasureSet &ms, float actualFlow)
{
	float avgVar = findAvgVariation(ms);
	if (avgVar == 0.0)
		return 0.0;
	return actualFlow / avgVar;
}

