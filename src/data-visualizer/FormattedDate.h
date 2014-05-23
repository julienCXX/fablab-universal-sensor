#ifndef _H_FORMATTED_DATE
#define _H_FORMATTED_DATE

#include <string>
#include <ostream>
#include <QDateTime>

// a simplified date format with string conversion
class FormattedDate
{
	private:
		QDateTime date;

	public:
		// default constructor
		FormattedDate();
		/*
		 * constructor from a formatted date string
		 * in format: "1111-12-31T23:59:59.123"
		 */
		FormattedDate(const std::string &str_date);
		// copy constructor
		FormattedDate(const FormattedDate &other);

		// comparaison operators
		bool operator==(const FormattedDate &other) const;
		bool operator!=(const FormattedDate &other) const;
		bool operator>(const FormattedDate &other) const;
		bool operator<(const FormattedDate &other) const;
		bool operator>=(const FormattedDate &other) const;
		bool operator<=(const FormattedDate &other) const;

		// flow operators
		std::ostream &operator<<(std::ostream &output) const;

		// prints like "1111-12-31 23:59:59.123"
		std::string toString() const;

		/*
		 * setting date from a formatted date string
		 * in format: "1111-12-31T23:59:59.123"
		 */
		void setFromString(const std::string &str_date);
};

#endif

