#include "parser/httpparser.h"
#include <iostream>
#include <algorithm>

namespace tenochtitlan
{
	namespace parser
	{
		using namespace std;

		shared_ptr<http::HttpEntity> HttpParser::Parse(string str)
		{
			shared_ptr<http::HttpEntity> entity = make_shared<http::HttpEntity>();
			unique_ptr<vector<string>> lines = Split(str, { "\10\13" });

			if (lines->size() > 0) {
				unique_ptr<vector<string>> tokens = Split(lines->at(0), { " " });
				if (tokens->size() == 3) {
					entity->SetMethod(tokens->at(0));
					entity->SetResourcePath(tokens->at(1));
					entity->SetVersion(tokens->at(2));
				}
			}
			for (unsigned int i = 1; i < lines->size(); i++) {
				unique_ptr<vector<string>> tokens = Split(lines->at(i), { ": " })
				if (tokens->size() == 2) {
					string header_name = tokens->at(0);
					if (header_name == "Host")
						entity->SetHost(tokens->at(1));
					else
						entity->AddHeader(header_name, tokens->at(1));
				}
			}
			return entity;
		}

		unique_ptr<vector<string>> HttpParser::Split(string str, vector<string> delims)
		{
			unique_ptr<vector<string>> result(new vector<string>());
			auto end = str.end();
			auto curr = str.begin();
			while (curr != end) {
				curr = find_if_not(curr, end, [&](char &c) -> bool 
					{
						for (unsigned int i = 0; i < delims.size(); i++)
							if (delims[i] == c)
								return true;
						return false;
					});

				if (curr == end)
					break;

				auto pivot = find_if(curr, end, [&](char &c) -> bool
					{
						for (unsigned int i = 0; i < delims.size(); i++)
							if (delims[i] == c)
								return true;
						return false;
					});

				result->push_back(string(curr, pivot));
				curr = pivot;
			}
			return result;
		}
	}
}
