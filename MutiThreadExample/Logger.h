#pragma once
#include <string>
#include <string_view>
#include <thread>
#include <mutex>
#include <queue>

class Logger
{
public:
	//�α� �׸� ���Ͽ� �����ϴ� ��׶��� �����带 ����
	Logger();
	
	//���� �� ���� ������ ����
	Logger(const Logger& src) = delete;
	Logger& operator=(const Logger& rhs) = delete;
	virtual ~Logger();

	//�α� �׸��� ť�� �����ϴ� �Լ�
	void log(std::string_view entry);

private:
	//��׶��� �������� ���� ���θ� ǥ���ϴ� �ο� ����
	bool exit_ = false;

	//��׶��� �����忡�� ������ �Լ�
	void processEntries();
	
	//queue ����ȭ�� ����� ���ؽ��� ���� ����
	std::mutex mutex_;
	std::condition_variable cond_var_;
	std::queue<std::string> queue_;

	std::thread thread_;
};

