#include <thread>
#include <string>
#include <mutex>
#include <iostream>
#include <vector>
#include "WRRMMap.h"

const int num_values = 10;

// test with hazard pointer map
void map_test(WRRMMap<int, int> &map, int num)
{

	for (int i = 0; i <= num_values; ++i)
	{
		// Old value if exists
		if (map.Lookup(i))
		{
			std::cout << "Old Key: " << i << " Thread: " << map.Lookup(i) << std::endl;
		}

		// Update to new value
		map.Update(i, num);

		// Lookup
		std::cout << "New Key: " << i << " Thread: " << map.Lookup(i) << std::endl;
	}
}

int main()
{
	WRRMMap<int, int> map;

	std::vector<std::thread> threads;

	for (int i = 0; i < 10; i++)
	{
		threads.emplace_back([&map, i]()
							 { map_test(map, i); });
	}

	for (auto &t : threads)
	{
		t.join();
	}

	threads.clear();

	// print out final values
	// numbers represents what thread updated it last
	for (int i = 1; i <= num_values; i++)
	{
		std::cout << "Key: " << i << "    Last thread: " << map.Lookup(i) << std::endl;
	}

	return 0;
}