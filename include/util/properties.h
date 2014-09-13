#ifndef _PROPERTIES_H_
#define _PROPERTIES_H_

#include <map>
#include <string>

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
		public:
			void Set(std::string, std::string);
			void Set(std::string, bool);
			void Set(std::string, int);
			void Set(std::string, double);

			bool GetBool(std::string, bool def = false);
			std::string GetString(std::string, const char *def = 0);
			int GetInt(std::string, int def = 0);
			double GetDouble(std::string, double def = 0);
		};
	}
}

#endif
