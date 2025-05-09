#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
	Semaphore(int count);

	void wait();
	void signal();
private:
	int count;
	std::mutex m;
	std::condition_variable cv;
};

#endif