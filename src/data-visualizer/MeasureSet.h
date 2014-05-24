#ifndef _H_MEASURE_SET
#define _H_MEASURE_SET

#include <map>
#include "FormattedDate.h"
#include "Measure.h"
#include "JsonMeasureFormat.h"

/*
 * a set of measures that can come from a file or several ones
 * the values are sorted by date, in ascending order
 */
class MeasureSet
{
	private:
		std::map<FormattedDate, Measure> measures;

	public:
		typedef std::map<FormattedDate, Measure>::iterator MSIt;
		typedef std::map<FormattedDate, Measure>::const_iterator MSCit;
		typedef std::map<FormattedDate, Measure>::reverse_iterator MSRit;

		// constructor
		MeasureSet();

		// adding measures to the set
		void add(const Measure &measure);
		// from a file, using a given file separator
		void add(const std::string &filePath, const char separ = ' ');

		// iterators
		MSIt begin();
		MSIt end();
		MSCit cbegin() const;
		MSCit cend() const;
		MSRit rbegin();
		MSRit rend();

		// formatted output
		std::string toJsonFormat(const JsonMeasureFormat &format) const;
};

#endif

