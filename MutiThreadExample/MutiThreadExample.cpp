#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "Logger.h"

void logSomeMessage(int id, Logger& logger)
{
    for (int i = 0; i < 10; ++i)
    {
        std::stringstream ss;
        ss << "Log entry" << i << " from thread " << id;
        logger.log(ss.str());
    }
}

int main()
{
    std::cout << "thread start!! \n";
    Logger logger;
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back(logSomeMessage, i, std::ref(logger));
    }

    for (auto& t : threads)
    {
        t.join();
    }
}


