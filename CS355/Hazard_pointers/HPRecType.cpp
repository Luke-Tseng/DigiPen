#include "HPRecType.h"

// per thread private variable
thread_local std::vector<void *> rlist;
HPRecType *HPRecType::pHead_ = nullptr;
int HPRecType::listLen_ = 0;
std::mutex HPRecType::rlistMutex;

void HPRecType::Retire(void *pOld)
{
    // put it in the retired list
    rlist.push_back(pOld);
    HPRecType::Scan(HPRecType::Head());
}

void HPRecType::Scan(HPRecType *head)
{
    std::lock_guard<std::mutex> lock(rlistMutex);
    // scan hazard pointers list
    std::set<void *> hp;
    while (head)
    {
        void *p = head->pHazard_;
        if (p)
        {
            hp.insert(p);
        }
        head = head->pNext_;
    }

    // search
    auto it = rlist.begin();
    while (it != rlist.end())
    {
        if (hp.find(*it) != hp.end())
        {
            auto p = static_cast<std::map<int, std::string> *>(*it);
            if (p) // Check if pointer is not null before deleting
            {
                delete p;
            }
            it = rlist.erase(it);
        }
        else
        {
            ++it;
        }
    }
}