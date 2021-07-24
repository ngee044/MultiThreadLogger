#include <iostream>
#include <string>
#include <vector>
#include <atomic>

#include "Logger.h"
#include "ThreadPool.h"

void logSomeMessage(int id, Logger& logger)
{
    for (int i = 0; i < 100; ++i)
    {
        std::stringstream ss;
        ss <<"Log entry" << i << " from thread " << id;
        logger.log(ss.str());
    }
}

void work(int t, int id)
{
    std::cout << id << " start " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(t));
    std::cout << id << " end after " << t << std::endl;
}

int work2(int t, int id)
{
    std::cout << id << " start " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(t));
    std::cout << id << " end after " << t << std::endl;
    
    return t + id;
}


int main()
{
    std::cout << "thread start!! " << std::endl;

#if 0 //write logger multithread example
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
#endif

#if 1 //thread Pool example

    const int thread_count = 3;
    ThreadPool pool(thread_count);

    //for (int i = 0; i < 10; ++i)
    //{
    //    std::function<void()> foo = [i]() { work(i % 3 + 1, i); };
    //    pool.EnqueueJob(foo);
    //}

    std::vector<std::future<int>> futures;
    for (int i = 0; i < 10; i++)
    {
        futures.emplace_back(pool.EnqueueJob(work2, i % thread_count + 1, i));
    }

    for (auto& f : futures)
    {
        std::cout << "result : " << f.get() << std::endl;
    }
#endif
}


