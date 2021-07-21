#pragma once
#include <string>
#include <string_view>
#include <thread>
#include <mutex>
#include <queue>

class Logger
{
public:
	//로그 항목 파일에 저장하는 백그라운드 스레드를 구동
	Logger();
	
	//복제 및 대입 연산자 삭제
	Logger(const Logger& src) = delete;
	Logger& operator=(const Logger& rhs) = delete;
	virtual ~Logger();

	//로그 항목을 큐에 저장하는 함수
	void log(std::string_view entry);

private:
	//백그라운드 스레드의 종료 여부를 표시하는 부울 변수
	bool exit_ = false;

	//백그라운드 스레드에서 실행할 함수
	void processEntries();
	
	//queue 동기화에 사용할 뮤텍스와 조건 변수
	std::mutex mutex_;
	std::condition_variable cond_var_;
	std::queue<std::string> queue_;

	std::thread thread_;
};

