#include "perm-jt.h"
#include <climits>

PermJohnsonTrotter::PermJohnsonTrotter(int size) : dir(size), element(size), size_(size)
{
	for(int i = 0;i < size;++i)
	{
		element[i] = i+1;
		dir[i] = -1;
	}
}

bool PermJohnsonTrotter::Next()
{
	int largest = INT_MIN;
	for(int i = 0;i < size_;++i)
	{
		if(dir[i] == 1)
		{
			if(i == size_-1) continue;
			if(element[i] > element[i+1])
			{
				if(largest == INT_MIN || element[largest] < element[i]) largest = i;
			}
		}
		if(dir[i] == -1)
		{
			if(i == 0) continue;
			if(element[i] > element[i-1])
			{
				if(largest == INT_MIN || element[largest] < element[i]) largest = i;
			}
		}
	}
	if(largest == INT_MIN)
	{
		return false;
	}
	int direction = dir[largest];
	std::swap(element[largest],element[largest+direction]);
	std::swap(dir[largest],dir[largest+direction]);
	if(element[largest+direction] < size_) 
	{
		for(int i = 0;i < size_;++i)
		{
			if(element[i] > element[largest+direction]) 
			{
				dir[i] = -dir[i];
			}
		}
	}

	return true;
}

std::vector<int> const &PermJohnsonTrotter::Get() const
{
	return element;
}