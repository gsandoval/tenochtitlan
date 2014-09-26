#include "util/stringutils.h"
#include <sstream>
#include <algorithm>

namespace tenochtitlan
{
	namespace util
	{
		using namespace std;

		vector<string> StringUtils::Split(const string &str, const string &sep)
		{
			vector<string> result;
			int begin = 0;
			int end = 0;
			while (end != string::npos) {
				begin = str.find_first_not_of(sep, end);
				end = str.find_first_of(sep, begin);
				if (begin != string::npos)
					result.push_back(str.substr(begin, end - begin));
			}
			return result;
		}

		bool StringUtils::IsInt(string &str)
		{
			stringstream ss;
			ss << str;
			int val = 0;
			ss >> val;
			bool valid = str.size() > 0 && ss.good();
			if (valid) {
				if (val == 0)
					for (unsigned int i = 0; i < str.size(); i++) {
						valid = str[i] == '0' || (i == 0 && (str[i] == '-' || str[i] == '+'));
						if (!valid)
							break;
					}
			}
			return valid;
		}

		bool StringUtils::IsDouble(string &str)
		{
			stringstream ss;
			ss << str;
			double val = 0;
			ss >> val;
			bool valid = str.size() > 0 && ss.good();
			if (valid) {
				if (val == 0) {
					int dot_count = 0;
					for (unsigned int i = 0; i < str.size(); i++) {
						if (str[i] == '.') {
							dot_count++;
						}
						valid = str[i] == '0' || (i == 0 && (str[i] == '-' || str[i] == '+')) || (str[i] == '.');
						if (!valid || dot_count > 1) {
							valid = false;
							break;
						}
					}
				}
			}
			return valid;
		}
		
		bool StringUtils::IsBool(string &str)
		{
			transform(str.begin(), str.end(), str.begin(), ::tolower);
			return str == "true" || str == "false";
		}
		
		bool StringUtils::IsRegex(string &str)
		{
			return true;
		}
		
		int StringUtils::ToInt(string &str)
		{
			stringstream ss;
			ss << str;
			int val = 0;
			ss >> val;
			return val;
		}
		
		double StringUtils::ToDouble(string &str)
		{
			stringstream ss;
			ss << str;
			double val = 0;
			ss >> val;
			return val;
		}
		
		bool StringUtils::ToBool(string &str)
		{
			transform(str.begin(), str.end(), str.begin(), ::tolower);
			return str == "true";
		}
		
		regex StringUtils::ToRegex(string &str)
		{
			return regex(str); 
		}
		
	}
}
