#include "FormattedDate.h"
#include <iostream>
#include <cassert>
#include <cmath>

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

float FormattedDate::absSecondsDiff(const FormattedDate &other) const
{
	qint64 thisMsEpoch = date.toMSecsSinceEpoch(),
	       otherMsEpoch = other.date.toMSecsSinceEpoch(),
	       milliDiff = thisMsEpoch - otherMsEpoch;
	float result = milliDiff / 1000.0;
	return abs(result);
}

bool FormattedDate::sameDayAs(const FormattedDate &other) const
{
	return date.daysTo(other.date) == 0;
}

string FormattedDate::toString() const
{
	static const char *format = "yyyy-MM-dd HH:mm:ss.zzz";
	static const QString qFormat = QString(format);
	return date.toString(qFormat).toStdString();
}

string FormattedDate::toStringDayOnly() const
{
	static const char *format = "yyyy-MM-dd";
	static const QString qFormat = QString(format);
	return date.toString(qFormat).toStdString();
}

void FormattedDate::setFromString(const string &str_date)
{
	static const char *format = "yyyy-M-dTHH:mm:ss.zzz";
	static const char *formatBis = "yyyy-M-dTH:m:s.zzz";
	static const char *formatSimp = "yyyy-M-dTHH:mm:ss";
	static const QString qFormat = QString(format);
	static const QString qFormatBis = QString(formatBis);
	static const QString qFormatSimp = QString(formatSimp);
	date = QDateTime::fromString(str_date.c_str(), qFormat);
	if (!date.isValid())
		// date with non padded h m s
		date = QDateTime::fromString(str_date.c_str(), qFormatBis);
	if (!date.isValid())
		// date without ms
		date = QDateTime::fromString(str_date.c_str(), qFormatSimp);
	assert(date.isValid());
}

