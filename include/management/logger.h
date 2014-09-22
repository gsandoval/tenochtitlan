#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <queue>
#include <condition_variable>
#include <mutex>

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

namespace tenochtitlan
{
	namespace management
	{
		void logger_ctrl_c_handler(int s);

		class Logger
		{
		private:
			std::string class_name;
			static std::queue<std::string> messages;
			static std::condition_variable messages_cond;
			static std::mutex messages_mutex;
			static std::mutex init_mutex;
			static bool running;
			static bool stopped;
			static bool inited;

			static void Start();
			static void Stop();

			friend void logger_ctrl_c_handler(int);
		public:
			Logger(std::string class_name);
			~Logger();

			void Debug(std::string mtd, std::string msg);
			void Info(std::string mtd, std::string msg);
			void Warn(std::string mtd, std::string msg);
			void Error(std::string mtd, std::string msg);
		};
	}
}

#endif
