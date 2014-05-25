#include "FormattedDate.h"
#include <iostream>
#include <cassert>

using namespace std;
using namespace Qt;

FormattedDate::FormattedDate()
{
}

FormattedDate::FormattedDate(const FormattedDate &other) : date(other.date)
{
}

FormattedDate::FormattedDate(const std::string &str_date)
{
	setFromString(str_date);
}

bool FormattedDate::operator==(const FormattedDate &other) const
{
	return date == other.date;
}

bool FormattedDate::operator!=(const FormattedDate &other) const
{
	return date != other.date;
}

bool FormattedDate::operator>(const FormattedDate &other) const
{
	return date > other.date;
}

bool FormattedDate::operator<(const FormattedDate &other) const
{
	return date < other.date;
}

bool FormattedDate::operator>=(const FormattedDate &other) const
{
	return date >= other.date;
}

bool FormattedDate::operator<=(const FormattedDate &other) const
{
	return date <= other.date;
}

ostream &FormattedDate::operator<<(std::ostream &output) const
{
	return output << toString();
}

string FormattedDate::toString() const
{
	static const char *format = "yyyy-MM-dd HH:mm:ss.zzz";
	static const QString qFormat = QString(format);
	return date.toString(qFormat).toStdString();
}

void FormattedDate::setFromString(const string &str_date)
{
	static const char *format = "yyyy-M-dTHH:mm:ss.zzz";
	static const char *formatSimp = "yyyy-M-dTHH:mm:ss";
	static const QString qFormat = QString(format);
	static const QString qFormatSimp = QString(formatSimp);
	date = QDateTime::fromString(str_date.c_str(), qFormat);
	if (!date.isValid())
		// date without ms
		date = QDateTime::fromString(str_date.c_str(), qFormatSimp);
	assert(date.isValid());
}

