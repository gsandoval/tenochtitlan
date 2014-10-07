#include "management/logger.h"
#include "management/applicationlifecyclelistener.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <signal.h>
#ifdef _MSC_VER
#include <windows.h>
#endif

namespace tenochtitlan
{
	namespace management
	{
		using namespace std;

		queue<string> Logger::messages;
		condition_variable Logger::messages_cond;
		mutex Logger::messages_mutex;
		mutex Logger::init_mutex;
		bool Logger::running = false;
		bool Logger::stopped = false;
		bool Logger::inited = false;

		void logger_ctrl_c_handler(int s)
		{
			Logger::Stop();
		}

		Logger::Logger(string class_name) : class_name(class_name)
		{
			unique_lock<mutex> lk(init_mutex);
			if (!inited) {
				inited = true;
				thread t = thread([&] {
					Start();
				});
				t.detach();

#ifndef _MSC_VER
				struct sigaction sig_int_handler;
				sig_int_handler.sa_handler = logger_ctrl_c_handler;
				sigemptyset(&sig_int_handler.sa_mask);
				sig_int_handler.sa_flags = 0;
				sigaction(SIGINT, &sig_int_handler, NULL);
#else
				(void) signal(SIGINT, logger_ctrl_c_handler);
#endif
			}
			lk.unlock();
		}

		Logger::~Logger()
		{
		}

		void Logger::LogHeader(ostringstream &oss, string level, string &mtd)
		{
			chrono::system_clock::time_point now = chrono::system_clock::now();
			time_t tt = chrono::system_clock::to_time_t(now);
			char mbstr[100];
#ifdef _MSC_VER
			strftime(mbstr, sizeof(mbstr), "%y-%m-%d %H:%M:%S", localtime(&tt));
#else
			strftime(mbstr, sizeof(mbstr), "%F %T", localtime(&tt));
#endif

			oss << mbstr << " [" << level << "] " << this_thread::get_id() << " [" << class_name << "::" << mtd << "] ";
		}

		void Logger::Debug(string mtd, string msg)
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
			oss << mbstr << " [DEBUG] " << this_thread::get_id() << " [" << class_name << "::" << mtd << "] " << msg;

			unique_lock<mutex> lk(messages_mutex);
			messages.push(oss.str());
			lk.unlock();

			messages_cond.notify_all();
		}

		void Logger::Warn(string mtd, string msg)
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
			oss << mbstr << " [WARN] " << this_thread::get_id() << " [" << class_name << "::" << mtd << "] " << msg;

			unique_lock<mutex> lk(messages_mutex);
			messages.push(oss.str());
			lk.unlock();

			messages_cond.notify_all();
		}

		void Logger::Error(string mtd, string msg)
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
			oss << mbstr << " [ERROR] " << this_thread::get_id() << " [" << class_name << "::" << mtd << "] " << msg;

			unique_lock<mutex> lk(messages_mutex);
			messages.push(oss.str());
			lk.unlock();

			messages_cond.notify_all();
		}

		void Logger::Write(ostringstream &oss, const char* format)
		{
			oss << format;
		}

		void Logger::Start()
		{
			running = true;
			unique_lock<mutex> lk(messages_mutex, defer_lock);
			while (running) {
				lk.lock();
				if (messages.empty())
					messages_cond.wait(lk);
				if (!running) {
					lk.unlock();
					break;
				}

				queue<string> local = messages;
				while (!messages.empty()) messages.pop();
				lk.unlock();

				while (!local.empty()) {
					cout << local.front() << endl;
					local.pop();
				}
			}
			stopped = true;
		}

		void Logger::Stop()
		{
			if (!running)
				return;

			running = false;
			messages_cond.notify_all();

			while (!stopped)
				this_thread::sleep_for(chrono::milliseconds(100));
		}
	}
}
