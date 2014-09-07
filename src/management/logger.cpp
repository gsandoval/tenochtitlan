#include "management/logger.h"
#include "management/applicationlifecyclelistener.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <signal.h>

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


				struct sigaction sig_int_handler;
				sig_int_handler.sa_handler = logger_ctrl_c_handler;
				sigemptyset(&sig_int_handler.sa_mask);
				sig_int_handler.sa_flags = 0;
				sigaction(SIGINT, &sig_int_handler, NULL);
			}
			lk.unlock();
		}

		Logger::~Logger()
		{
		}

		void Logger::Debug(string mtd, string msg)
		{
			ostringstream oss;
			oss << "[DEBUG] " << this_thread::get_id() << " [" << class_name << "::" << mtd << "] " << msg;

			unique_lock<mutex> lk(messages_mutex);
			messages.push(oss.str());
			lk.unlock();

			messages_cond.notify_all();
		}

		void Logger::Info(string mtd, string msg)
		{
			ostringstream oss;
			oss << "[INFO] " << this_thread::get_id() << " [" << class_name << "::" << mtd << "] " << msg;

			unique_lock<mutex> lk(messages_mutex);
			messages.push(oss.str());
			lk.unlock();

			messages_cond.notify_all();
		}

		void Logger::Warn(string mtd, string msg)
		{
			ostringstream oss;
			oss << "[WARN] " << this_thread::get_id() << " [" << class_name << "::" << mtd << "] " << msg;

			unique_lock<mutex> lk(messages_mutex);
			messages.push(oss.str());
			lk.unlock();

			messages_cond.notify_all();
		}

		void Logger::Error(string mtd, string msg)
		{
			ostringstream oss;
			oss << "[ERROR] " << this_thread::get_id() << " [" << class_name << "::" << mtd << "] " << msg;

			unique_lock<mutex> lk(messages_mutex);
			messages.push(oss.str());
			lk.unlock();

			messages_cond.notify_all();
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
