#ifndef _H_JSON_MEASURE_FORMAT
#define _H_JSON_MEASURE_FORMAT

#include <string>
#include <map>

// specifies format to be used when printing measure data to the JSON format
class JsonMeasureFormat
{
	private:
		const std::string EMPTY_STRING;
		std::string dateFormat;
		bool isBoolResult;

		std::string nonBoolResultField;
		std::string boolTrueResultField = "hOn";
		std::string boolFalseResultField = "hOff";

		std::map<int, std::string> valuesFields;

	public:
		JsonMeasureFormat();

		void setDateFormat(const std::string &format);
		const std::string &getDateFormat() const;

		void setIsBoolResult(bool mode);
		bool getIsBoolResult() const;

		void setNonBoolResultField(const std::string &field);
		const std::string &getNonBoolResultField() const;

		void setBoolTrueResultField(const std::string &field);
		const std::string &getBoolTrueResultField() const;

		void setBoolFalseResultField(const std::string &field);
		const std::string &getBoolFalseResultField() const;

		void setValueField(int index, const std::string &field);
		const std::string &getValueField(int index) const;
};

#endif

