#include <iostream> // std::cout
#include <atomic>	// std::atomic
#include <thread>	// std::thread
#include <vector>	// std::vector
#include <deque>	// std::deque
#include <mutex>	// std::mutex

struct Pair
{
	std::vector<int> *pointer;
	long ref_count;
}; // __attribute__((aligned(16),packed));
// for some compilers alignment needed to stop std::atomic<Pair>::load to segfault

std::atomic<int> counter(0);

class MemoryBank
{
private:
	std::deque<std::vector<int> *> bank;
	std::mutex m;

public:
	MemoryBank() : bank(1000)
	{
		for (int i = 0; i < 1000; ++i)
		{
			bank[i] =
				reinterpret_cast<std::vector<int> *>(new char[sizeof(std::vector<int>)]);
		}
	}

	~MemoryBank()
	{
		for (auto &el : bank)
		{
			delete[] reinterpret_cast<char *>(el);
		}
	}

	std::vector<int> *Get()
	{
		std::lock_guard<std::mutex> lock(m);
		std::vector<int> *p = bank[0];
		bank.pop_front();
		return p;
	}

	void Store(std::vector<int> *p)
	{
		std::lock_guard<std::mutex> lock(m);
		bank.push_back(p);
	}
};
class LFSV
{
private:
	MemoryBank mb;
	std::atomic<std::vector<int> *> pdata;
	std::mutex wr_mutex;

public:
	LFSV() : mb(), pdata(new(mb.Get()) std::vector<int>),
			 wr_mutex()
	{
	}

	~LFSV()
	{
		pdata.load()->~vector();
		mb.Store(pdata.load());
	}

	void Insert(int const &v)
	{
		std::vector<int> *pdata_new = nullptr, *pdata_old;
		do
		{
			++counter;
			// delete pdata_new;
			if (pdata_new)
			{
				pdata_new->~vector();
				mb.Store(pdata_new);
			}

			pdata_old = pdata;
			pdata_new = new (mb.Get()) std::vector<int>(*pdata_old);
			std::vector<int>::iterator b = pdata_new->begin();
			std::vector<int>::iterator e = pdata_new->end();

			// first in empty or last element
			if (b == e || v >= pdata_new->back())
			{
				pdata_new->push_back(v);
			}
			else
			{
				for (; b != e; ++b)
				{
					if (*b >= v)
					{
						pdata_new->insert(b, v);
						break;
					}
				}
			}

		} while (!(this->pdata).compare_exchange_weak(pdata_old, pdata_new));

		pdata_old->~vector();
		mb.Store(pdata_old);
	}

	int const &operator[](int pos) const
	{
		return (*pdata)[pos];
	}
};