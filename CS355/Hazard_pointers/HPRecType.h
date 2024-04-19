#ifndef HPRECTYPE_H
#define HPRECTYPE_H
#include <atomic>
#include <algorithm>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <mutex>

template <class T>
bool CAS(T *addr, T expected, T value)
{
	if (*addr == expected)
	{
		*addr = value;
		return true;
	}
	return false;
}

class HPRecType
{
	HPRecType *pNext_;
	int active_;
	// Global header of the HP list
	static HPRecType *pHead_;
	// The length of the list
	static int listLen_;

	static std::mutex rlistMutex;

	static void Scan(HPRecType *head);

public:
	// Can be used by the thread that acquired it
	void *pHazard_;

	static HPRecType *Head()
	{
		return pHead_;
	}

	// Acquires one hazard pointer
	static HPRecType *Acquire()
	{
		// Try to reuse a retired HP record
		HPRecType *p = pHead_;
		for (; p; p = p->pNext_)
		{
			if (p->active_ || !CAS(&p->active_, 0, 1))
				continue;
			// Got one!
			return p;
		}

		// increment the list length
		int oldLen;
		do
		{
			oldLen = listLen_;
		} while (!CAS(&listLen_, oldLen, oldLen + 1));

		// Allocate a new one
		HPRecType *pNew = new HPRecType;
		pNew->active_ = 1;
		pNew->pHazard_ = 0;

		// push it to the front
		do
		{
			p = pHead_;
			pNew->pNext_ = p;
		} while (!CAS(&pHead_, p, pNew));

		return pNew;
	}

	// Releases a hazard pointer
	static void Release(HPRecType *p)
	{
		p->pHazard_ = 0;
		p->active_ = 0;
	}

	static void Retire(void *pOld);
};

#endif