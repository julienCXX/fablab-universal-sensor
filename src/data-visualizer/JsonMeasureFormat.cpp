#include "JsonMeasureFormat.h"

using namespace std;

JsonMeasureFormat::JsonMeasureFormat() : isBoolResult(true)
{
}

void JsonMeasureFormat::setDateFormat(const string &format)
{
	dateFormat = format;
}

const string &JsonMeasureFormat::getDateFormat() const
{
	return dateFormat;
}

void JsonMeasureFormat::setIsBoolResult(bool mode)
{
	isBoolResult = mode;
}

bool JsonMeasureFormat::getIsBoolResult() const
{
	return isBoolResult;
}

void JsonMeasureFormat::setNonBoolResultField(const string &field)
{
	nonBoolResultField = field;
}

const string &JsonMeasureFormat::getNonBoolResultField() const
{
	return nonBoolResultField;
}

void JsonMeasureFormat::setBoolTrueResultField(const string &field)
{
	boolTrueResultField = field;
}

const string &JsonMeasureFormat::getBoolTrueResultField() const
{
	return boolTrueResultField;
}

void JsonMeasureFormat::setBoolFalseResultField(const string &field)
{
	boolFalseResultField = field;
}

const string &JsonMeasureFormat::getBoolFalseResultField() const
{
	return boolFalseResultField;
}

void JsonMeasureFormat::setValueField(int index, const string &field)
{
	valuesFields.insert(pair<int, string>(index, field));
}

const string &JsonMeasureFormat::getValueField(int index) const
{
	if (valuesFields.count(index) == 0)
		return EMPTY_STRING;
	return valuesFields.find(index)->second;
}

