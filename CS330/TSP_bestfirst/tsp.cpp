#include "tsp.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <map>
#include <algorithm>

void read2(char const *filename, MAP &map, int &TotalCity)
{
	map.clear();
	std::ifstream in(filename, std::ifstream::in);
	if (!in.is_open())
	{
		std::cout << "problem reading " << filename << std::endl;
		return;
	}
	in >> TotalCity;
	for (int i = 0; i < TotalCity; ++i)
	{
		std::vector<int> row;
		for (int j = 0; j < TotalCity; ++j)
		{
			row.push_back(std::numeric_limits<int>::max());
		}
		map.push_back(row);
	}
	for (int i = 0; i < TotalCity; ++i)
	{
		for (int j = i + 1; j < TotalCity; ++j)
		{
			if (!in.good())
			{
				std::cout << "problem reading " << filename << std::endl;
				return;
			}
			in >> map[i][j];
			map[j][i] = map[i][j];
		}
	}
}

int calculateLowerBound(MAP const &map, int const &numCities, std::vector<int> &currSolution, int const &currCost)
{
	int lowerBound = currCost;
	for (int i = 0; i < numCities; ++i)
	{
		if (std::find(currSolution.begin(), currSolution.end() - 1, i) == currSolution.end() - 1)
		{
			int rowMin = std::numeric_limits<int>::max();
			for (int j = 0; j < numCities; ++j)
			{
				if (j != currSolution.back() && i != j)
				{
					if (rowMin > map[i][j])
						rowMin = map[i][j];
				}
			}
			lowerBound += rowMin;
		}
	}
	return lowerBound;
}

// recursive SolveTSP
void SolveTSP(MAP const &map, int const &numCities, int const &index, std::vector<int> &currSolution,
			  int &currCost, std::vector<int> &bestSolution, int &bestCost)
{
	// termination check (if on last index)
	if (index == numCities)
	{
		if (currCost < bestCost)
		{
			bestSolution = currSolution;
			bestCost = currCost;
		}
		return;
	}

	// hack - use map to order, key - bound, value - job.
	// insert pairs, then traverse the map in-order
	std::multimap<int, int> orderedCities;

	if (index == numCities - 1)
	{
		// going back to city 0
		int lastCity = currSolution.back();
		currSolution.push_back(0);
		currCost += map[lastCity][0];

		int lb = calculateLowerBound(map, numCities, currSolution, currCost);
		orderedCities.insert(std::pair<int, int>(lb, 0));
		currSolution.pop_back();
		currCost -= map[lastCity][0];
	}
	else
	{
		// generate nodes and order them by bound
		for (int j = 0; j < numCities; ++j)
		{
			// skip if job j is already assigned
			if (std::find(currSolution.begin(), currSolution.end(), j) != currSolution.end())
				continue;
			int lastCity = currSolution.back();
			currSolution.push_back(j);
			currCost += map[lastCity][j];
			int lb = calculateLowerBound(map, numCities, currSolution, currCost);
			orderedCities.insert(std::pair<int, int>(lb, j));
			currSolution.pop_back();
			currCost -= map[lastCity][j];
		}
	}

	// traverse nodes in the above order - BEST FIRST
	std::multimap<int, int>::const_iterator b = orderedCities.begin(),
											e = orderedCities.end();
	for (; b != e; ++b)
	{
		int j = b->second; // get index;
		int lastCity = currSolution.back();
		currSolution.push_back(j);
		currCost += map[lastCity][j];

		int lb = b->first;
		// branch cancelation check
		// std::cout << "lowerBound = " << lowerBound( m, N, index, currSolution, currCost ) << std::endl;
		if (lb < bestCost)
		{
			SolveTSP(map, numCities, index + 1, currSolution, currCost, bestSolution, bestCost);
		}

		currSolution.pop_back();
		currCost -= map[lastCity][j];
	}
}

std::vector<int> SolveTSP(char const *filename)
{
	MAP map;
	int numCities;
	read2(filename, map, numCities);

	std::vector<int> currSolution;
	int currCost = 0;
	std::vector<int> bestSolution;
	int bestCost = std::numeric_limits<int>::max();
	currSolution.push_back(0);
	SolveTSP(map, numCities, 0, currSolution, currCost, bestSolution, bestCost);
	return bestSolution;
}
