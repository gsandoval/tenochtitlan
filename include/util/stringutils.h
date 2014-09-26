#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <string>
#include <vector>
#include <regex>

namespace tenochtitlan
{
	namespace util
	{
		class StringUtils
		{
		public:
			static std::vector<std::string> Split(const std::string &str, const std::string &sep);
			static bool IsInt(std::string &str);
			static bool IsDouble(std::string &str);
			static bool IsBool(std::string &str);
			static bool IsRegex(std::string &str);
			static int ToInt(std::string &str);
			static double ToDouble(std::string &str);
			static bool ToBool(std::string &str);
			static std::regex ToRegex(std::string &str);
		};
	}
}

#endif
