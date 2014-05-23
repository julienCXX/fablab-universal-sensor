#include "FormattedDate.h"

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
	static const char *format = "yyyy:MM:Dd HH:mm:ss.zzz";
	static const QString qFormat = QString(format);
	return date.toString(qFormat).toStdString();
}

void FormattedDate::setFromString(const string &str_date)
{
	static const char *format = "yyyy:M:dTH:m:s.zzz";
	static const QString qFormat = QString(format);
	date = QDateTime::fromString(QString(str_date.c_str()), qFormat);
}


