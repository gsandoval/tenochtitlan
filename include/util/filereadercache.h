#ifndef _FILE_READER_CACHE_H_
#define _FILE_READER_CACHE_H_

#include "http/content/httpfilecontent.h"
#include <string>
#include <map>
#include <memory>
#include <mutex>

namespace tenochtitlan
{
	namespace util
	{
		class FileReaderCache
		{
		private:
			std::map<std::string, std::shared_ptr<http::content::HttpFileContent>> file_cache;
			std::map<std::string, long long> file_timestamps;
			std::mutex file_cache_mutex;

			FileReaderCache();
			std::string GetFileExtension(std::string &filepath);
		public:
			static FileReaderCache* Instance()
			{
				static FileReaderCache instance;
				return &instance;
			}
			bool Exists(const std::string &path);
			std::shared_ptr<http::content::HttpFileContent> GetFile(std::string);
		};
	}
}

#endif
