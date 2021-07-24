#include <iostream>

#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t thread_count) : num_threads_count_(thread_count), stop_all_(false)
{
	worker_threads_.reserve(num_threads_count_);
	for (size_t i = 0; i < num_threads_count_; ++i)
	{
		worker_threads_.emplace_back([this]() {
			this->WorkerThread();
			});
	}
}

ThreadPool::~ThreadPool()
{
	stop_all_ = true;
	cndvar_job_q_.notify_all();

	for (auto& t : worker_threads_)
	{
		t.join();
	}
}

void ThreadPool::WorkerThread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(mutext_job_q_);
		cndvar_job_q_.wait(lock, [this]() { return !(this->q_jobs_.empty()) || stop_all_; });
		if (stop_all_ && this->q_jobs_.empty())
		{
			return;
		}

		std::function<void()> job = std::move(q_jobs_.front());
		q_jobs_.pop();
		lock.unlock();

		//해당 쓰레드의 함수(job)를 실행
		job();
	}
}

//void ThreadPool::EnqueueJob(std::function<void()> job)
//{
//	if (stop_all_)
//	{
//		throw std::runtime_error("stop using ThreadPool");
//	}
//	{
//		std::lock_guard<std::mutex> lock(mutext_job_q_);
//		q_jobs_.push(std::move(job));
//	}
//	cndvar_job_q_.notify_one();
//}

template <class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type> ThreadPool::EnqueueJob(F&& f, Args&&... args) 
{
	if (stop_all_)
	{
		throw std::runtime_error("stop using ThreadPool");
	}

	using return_type = typename std::result_of<F(Args...)>::type;
	// std::packaged_task<return_type()> job(std::bind(f, args...)); 객체가 파괴되어버린다...
	auto job = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

	std::future<return_type> job_result_future = job->get_future();
	{
		std::lock_guard<std::mutex> lock(mutext_job_q_);
		//jobs_.push([&job]() { job(); }); //이때 객체가 파괴됨....
		q_jobs_.push([job]() { (*job)(); });
	}
	cndvar_job_q_.notify_one();

	return job_result_future;

}





