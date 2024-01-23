#include "perm-lexicographical.h"
#include <algorithm>
#include <iostream>

// checks if its lexicographical
bool isLexicographical(const std::vector<int>& vec) {
	int n = vec.size();
    for (int i = 0; i < n-1; ++i)
	{
        if (vec[i] < vec[i + 1]) {
            return false;
        }
    }
    return true;
}

bool nextPermLexicographical (std::vector<int> & p) {
	if(isLexicographical(p)) return false;
	int n = p.size();
	int p2 = n-1;
	int p1 = n-2;
	for(int i = n-2;i>=0;--i)
	{
		if(p[i] < p[p2])
		{
			p1 = i;
			break;
		}
		p2 = i;
	}
	int smallest = p2;
	for(int j = p2;j<n;++j)
	{
		if(p[j] > p[p1] && p[smallest] > p[j]) smallest = j;
	}
	std::swap(p[p1],p[smallest]);
	for(int i = p2;i < n-1;++i)
	{
		int mini = i;
		for(int j = i+1;j < n;++j)
		{
			if(p[j] < p[mini])
			{
				mini = j;
			}
		}
		if(mini != i)
		{
			std::swap(p[i],p[mini]);
		}
	}

	return true;
}
