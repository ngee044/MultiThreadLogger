#pragma once

#include <thread>
#include <future>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <cstdio>
#include <chrono>
#include <memory>
#include <condition_variable>

class ThreadPool
{
public:
	ThreadPool(size_t thread_count);
	~ThreadPool();

	//void EnqueueJob(std::function<void()> job);

	template <class F, class... Args>
	std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(F&& f, Args&&... args);

protected:
	void WorkerThread();

private:
	size_t num_threads_count_ = 0;
	std::vector<std::thread> worker_threads_;

	std::queue<std::function<void()>> q_jobs_;

	//생산자-소비자 패턴 구현
	std::condition_variable cndvar_job_q_;
	std::mutex mutext_job_q_;

	//모든 스레드 종료
	bool stop_all_;
};

