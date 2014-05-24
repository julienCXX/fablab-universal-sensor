#ifndef _H_MEASURE
#define _H_MEASURE

#include <string>
#include <vector>
#include "FormattedDate.h"
#include "JsonMeasureFormat.h"

/*
 * a representation of a SensorTag measure (several values possibles),
 * at a given date, with an associated result
*/
class Measure
{
	private:
		FormattedDate date;
		std::vector<float> values;
		float result;

		static std::vector<std::string> split(const std::string &s,
				char separator);
		Measure();

	public:
		/*
		 * Constructs a measure from a string in the format
		 * "date value value ... value".
		 * Each field is separated by a separator.
		 */
		Measure(const std::string &line, char separator = ' ',
				float result = 0.0);

		// returns the date member
		const FormattedDate &getDate() const;
		// returns the list of values member
		const std::vector<float> &getValues() const;
		// returns the number of values contained in the value member
		int nbOfValues() const;
		// filtering result getters and setters
		bool getBoolResult() const;
		void setResult(bool value);
		float getFloatResult() const;
		void setResult(float value);

		// comparaison operators
		bool operator==(const Measure &other) const;
		bool operator!=(const Measure &other) const;
		bool operator>(const Measure &other) const;
		bool operator<(const Measure &other) const;
		bool operator>=(const Measure &other) const;
		bool operator<=(const Measure &other) const;

		// direct access to value: 0 == 1st element of "values"
		float operator[](int index) const;

		// prints without opening "{" and closing "}"
		std::string toJsonFormat(const JsonMeasureFormat &format) const;
};

#endif

