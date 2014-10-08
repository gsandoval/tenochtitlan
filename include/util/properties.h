#ifndef _PROPERTIES_H_
#define _PROPERTIES_H_

#include <map>
#include <string>
#include <regex>
#include <memory>

namespace tenochtitlan
{
	namespace util
	{
		class Properties
		{
		private:
			std::map<std::string, std::string> strings;
			std::map<std::string, bool> bools;
			std::map<std::string, int> ints;
			std::map<std::string, double> doubles;
			std::map<std::string, std::shared_ptr<std::regex>> regexs;
		public:
			void Set(std::string, std::string);
			void Set(std::string, bool);
			void Set(std::string, int);
			void Set(std::string, double);
			void Set(std::string, std::shared_ptr<std::regex> value);

			bool HasString(std::string name);
			bool HasBool(std::string name);
			bool HasInt(std::string name);
			bool HasDouble(std::string name);
			bool HasRegex(std::string name);

			bool Has(std::string name);

			void AddAll(std::shared_ptr<Properties> p);

			bool GetBool(std::string, bool def = false);
			std::string GetString(std::string, const char *def = 0);
			int GetInt(std::string, int def = 0);
			double GetDouble(std::string, double def = 0);
			std::shared_ptr<std::regex> GetRegex(std::string);
		};
	}
}

#endif
