#include "util/properties.h"

namespace tenochtitlan
{
	namespace util
	{
		using namespace std;

		void Properties::Set(string name, string value)
		{
			strings[name] = value;
		}

		void Properties::Set(string name, bool value)
		{
			bools[name] = value;
		}

		void Properties::Set(string name, int value)
		{
			ints[name] = value;
		}
		
		void Properties::Set(string name, double value)
		{
			doubles[name] = value;
		}

		bool Properties::GetBool(string name, bool def)
		{
			bool value = def;
			if (bools.find(name) != bools.end())
				value = bools[name];
			return value;
		}
		
		string Properties::GetString(string name, const char *def)
		{
			string value = "";
			if (def)
				value = string(def);
			if (strings.find(name) != strings.end())
				value = strings[name];
			return value;
		}
		
		int Properties::GetInt(string name, int def)
		{
			int value = def;
			if (ints.find(name) != ints.end())
				value = ints[name];
			return value;
		}
		
		double Properties::GetDouble(string name, double def)
		{
			double value = def;
			if (doubles.find(name) != doubles.end())
				value = doubles[name];
			return value;
		}
	}
}
