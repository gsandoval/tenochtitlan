#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <sstream>

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

#define LogDebug(...) Debug(__func__, __VA_ARGS__)
#define LogInfo(...) Info(__func__, __VA_ARGS__)
#define LogError(...) Error(__func__, __VA_ARGS__)
#define LogWarn(...) Warn(__func__, __VA_ARGS__)

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

			void Write(std::ostringstream &oss, const char* format);

			template<typename T, typename... Ts>
			void Write(std::ostringstream &oss, const char* format, T value, Ts... Fargs)
			{
				for ( ; *format != '\0'; format++) {
			        if ( *format == '\@' ) {
			           oss << value;
			           Write(format + 1, Fargs...);
			           return;
			        }
			        oss << *format;
			    }
			}

			void LogHeader(std::ostringstream oss, std::string level, std::string mtd, std::string msg)
			{
				std::chrono::system_clock::time_point now = chrono::system_clock::now();
				time_t tt = chrono::system_clock::to_time_t(now);
				char mbstr[100];
#ifdef _MSC_VER
				strftime(mbstr, sizeof(mbstr), "%y-%m-%d %H:%M:%S", localtime(&tt));
#else
				strftime(mbstr, sizeof(mbstr), "%F %T", localtime(&tt));
#endif

				std::ostringstream oss;
				oss << mbstr << " [" << level << "] " << this_thread::get_id() << " [" << class_name << "::" << mtd << "] " << msg;
				Write(oss, msg.c_str());

				std::unique_lock<mutex> lk(messages_mutex);
				messages.push(oss.str());
				lk.unlock();

				messages_cond.notify_all();
			}

			friend void logger_ctrl_c_handler(int);
		public:
			Logger(std::string class_name);
			~Logger();

			void Debug(std::string mtd, std::string msg);
			void Info(std::string mtd, std::string msg)
			{

			}

			template<typename T, typename... Ts>
			void Info(std::string mtd, std::string msg, T value, Ts... Fargs)
			{
				chrono::system_clock::time_point now = chrono::system_clock::now();
				time_t tt = chrono::system_clock::to_time_t(now);
				char mbstr[100];
#ifdef _MSC_VER
				strftime(mbstr, sizeof(mbstr), "%y-%m-%d %H:%M:%S", localtime(&tt));
#else
				strftime(mbstr, sizeof(mbstr), "%F %T", localtime(&tt));
#endif

				ostringstream oss;
				Write(oss, msg.c_str());
				oss << mbstr << " [INFO] " << this_thread::get_id() << " [" << class_name << "::" << mtd << "] " << msg;

				unique_lock<mutex> lk(messages_mutex);
				messages.push(oss.str());
				lk.unlock();

				messages_cond.notify_all();
			}
			void Warn(std::string mtd, std::string msg);
			void Error(std::string mtd, std::string msg);
		};
	}
}

#endif
