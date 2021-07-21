#include <memory>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>

#include "Logger.h"

using namespace std;

Logger::Logger()
{
	thread_ = std::thread{ &Logger::processEntries, this };
}

Logger::~Logger()
{
	{
		std::unique_lock lock(mutex_);
		exit_ = true; //스레드에 알림을 보내 스레드 정상 종료
		cond_var_.notify_all(); //스레드를 깨우도록 조건 변수에 알림을 보낸다.
	}
	thread_.join();
}

void Logger::log(std::string_view entry)
{
	std::unique_lock lock(mutex_);
	queue_.push(std::string(entry));

	cond_var_.notify_all();
}

void Logger::processEntries()
{
	std::ofstream logFile("log.txt");
	if (logFile.fail())
	{
		std::cerr << "Failed to open logfile" << std::endl;
		return;
	}

	std::unique_lock lock(mutex_);
	while (true)
	{
		std::this_thread::sleep_for(1000ms);
		if (!exit_) cond_var_.wait(lock);

		lock.unlock();
		while (true)
		{
			lock.lock();
			if (queue_.empty())
			{
				break;
			}
			else
			{
				logFile << queue_.front() << std::endl;
				queue_.pop();
			}
			lock.unlock();
		}
		if (exit_) break;
	}
}
