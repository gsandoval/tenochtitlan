#include "util/filereadercache.h"
#include <algorithm>
#include <cstdio>
#include <fstream>

namespace tenochtitlan
{
	namespace util
	{
		using namespace std;

		FileReaderCache::FileReaderCache()
		{

		}

		shared_ptr<http::content::HttpFileContent> FileReaderCache::GetFile(string path)
		{
			unique_lock<mutex> lk(file_cache_mutex);

			bool cached = file_cache.find(path) != file_cache.end();
			if (!cached) {
				ifstream file(path, ifstream::binary);
				file.seekg(0, file.end);
				streamsize size = file.tellg();
				file.seekg(0, file.beg);

				char *file_bytes = new char[size];
				file.read(file_bytes, size);
				file.close();

				auto buffer = shared_ptr<socket::Buffer>(new socket::Buffer(file_bytes, size));

				auto content = shared_ptr<http::content::HttpFileContent>(new http::content::HttpFileContent(buffer));

				string ext = GetFileExtension(path);
				content->SetMimeForExt(ext);

				file_cache[path] = content;
			}

			return file_cache[path];
		}

		string FileReaderCache::GetFileExtension(string &filepath)
		{
			static string dot = ".";
			auto s = find_end(filepath.begin(), filepath.end(), dot.begin(), dot.end());
			return string(s, filepath.end());
		}

		bool FileReaderCache::Exists(const string &path)
		{
			if (FILE *file = fopen(path.c_str(), "r")) {
				fclose(file);
				return true;
		    } else {
		        return false;
		    }
		    /*
		    struct stat s;
			if( stat(path,&s) == 0 )
			{
			    if( s.st_mode & S_IFDIR )
			    {
			        //it's a directory
			    }
			    else if( s.st_mode & S_IFREG )
			    {
			        //it's a file
			    }
			    else
			    {
			        //something else
			    }
			}
			else
			{
			    //error
			}
		    */
		}
	}
}
