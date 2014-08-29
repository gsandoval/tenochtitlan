#include "parser/httpparser.h"
#include <iostream>

namespace tenochtitlan
{
	namespace parser
	{
		using namespace std;

		shared_ptr<http::HttpEntity> HttpParser::Parse(string str)
		{
			shared_ptr<http::HttpEntity> entity = make_shared<http::HttpEntity>();
			cout << str << endl;
			return entity;
		}
	}
}
