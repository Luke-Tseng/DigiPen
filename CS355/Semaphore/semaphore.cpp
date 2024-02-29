#include "semaphore.h"

Semaphore::Semaphore(int count)
{
    this->count = count;
}

void Semaphore::wait()
{
    std::unique_lock<std::mutex> lock(m);
    while (count == 0)
    {
        cv.wait(lock);
    }
    count--;
}

void Semaphore::signal()
{
    std::unique_lock<std::mutex> lock(m);
    count++;
    cv.notify_one();
}
