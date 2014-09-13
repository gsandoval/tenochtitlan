#include "parser/httpparser.h"
#include <iostream>
#include <algorithm>

namespace tenochtitlan
{
	namespace parser
	{
		using namespace std;

		shared_ptr<socket::Buffer> HttpParser::Parse(queue<shared_ptr<socket::Buffer>> &buffers, shared_ptr<http::HttpEntity> entity)
		{
			static char LF = 10;
			static char CR = 13;

			int curr_line = 0;
			string name, value;

			int buffer_size = 0;
			char *buffer;

			queue<shared_ptr<socket::Buffer>> copy = buffers;
			while (!copy.empty()) {
				buffer_size += copy.front()->Size();
				copy.pop();
			}

			int offset = 0;
			buffer = new char[buffer_size];
			while (!buffers.empty()) {
				memcpy(buffer + offset, buffers.front()->Buf(), buffers.front()->Size());
				offset += buffers.front()->Size();
				buffers.pop();
			}
			
			int token_count = 0;
			int* starts = new int[3];
			int* ends = new int[3];

			int last_pos_processed = 0;
			bool token_s_found = false;
			int consecutive_lf = 0;
			for (int j = 0; j < buffer_size; j++) {
				if (buffer[j] == LF || buffer[j] == CR) {
					if (buffer[j] == LF) {
						consecutive_lf++;
					}

					if (consecutive_lf >= 2) {
						break;
					}

					if (token_s_found) {
						token_s_found = false;
						ends[token_count++] = j;
					}

					if (token_count == 0)
						continue;

					if (curr_line == 0 && token_count == 3) {
						entity->SetMethod(string(buffer + starts[0], buffer + ends[0]));
						entity->SetResourcePath(string(buffer + starts[1], buffer + ends[1]));
						entity->SetVersion(string(buffer + starts[2], buffer + ends[2]));
						last_pos_processed = ends[2] + 1;
					} else if (curr_line > 0 && token_count == 2) {
						entity->AddHeader(string(buffer + starts[0], buffer + ends[0]), string(buffer + starts[1], buffer + ends[1]));
						last_pos_processed = ends[1] + 1;
					}

					token_count = 0;
					curr_line++;
				} else if (curr_line == 0) {
					consecutive_lf = 0;
					if (buffer[j] == ' ') {
						if (token_s_found && token_count < 2) {
							token_s_found = false;
							ends[token_count++] = j;
						}
					} else {
						if (!token_s_found) {
							token_s_found = true;
							starts[token_count] = j;
						}
					}
				} else {
					consecutive_lf = 0;
					if (buffer[j] == ' ' || buffer[j] == ':') {
						if (token_s_found && token_count < 1) {
							token_s_found = false;
							ends[token_count++] = j;
						}
					} else {
						if (!token_s_found) {
							token_s_found = true;
							starts[token_count] = j;
						}
					}
				}
			}

			delete starts;
			delete ends;

			shared_ptr<socket::Buffer> buff;

			int remaining_byte_number = buffer_size - last_pos_processed;
			if (remaining_byte_number > 0) {
				char *remaining_bytes = new char[remaining_byte_number];
				buff = shared_ptr<socket::Buffer>(new socket::Buffer(remaining_bytes, remaining_byte_number));
			} else {
				buff = shared_ptr<socket::Buffer>(new socket::Buffer());
			}

			return buff;
		}

	}
}
