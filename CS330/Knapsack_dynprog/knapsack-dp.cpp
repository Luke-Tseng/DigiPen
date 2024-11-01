#include "knapsack-dp.h"
#include <iostream>
#include <numeric>

////////////////////////////////////////////////////////////
Item::Item(int const &weight, int const &value)
	: weight(weight), value(value)
{
}

////////////////////////////////////////////////////////////
Item::Item(Item const &original)
	: weight(original.weight), value(original.value)
{
}

////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &os, Item const &item)
{
	os << "(" << item.weight << " , " << item.value << ") ";
	return os;
}

////////////////////////////////////////////////////////////
std::istream &operator>>(std::istream &os, Item &item)
{
	os >> item.weight >> item.value;
	return os;
}

////////////////////////////////////////////////////////////
typedef std::vector<std::vector<int>> Table; // 2-dimensional table

////////////////////////////////////////////////////////////
// the returned value is a vector of indices
std::vector<int> knapsackDP(std::vector<Item> const &items, int const &W)
{
	int num_items = items.size();

	std::vector<std::vector<int>> table(W + 1, std::vector<int>(num_items + 1, 0));

	for (int w = 1; w <= W; ++w)
	{
		for (int n = 1; n <= num_items; ++n)
		{
			// item can fit
			if (items[n - 1].weight <= w)
			{
				table[w][n] = std::max(items[n - 1].value + table[w - items[n - 1].weight][n - 1], table[w][n - 1]);
			}
			else
			{
				table[w][n] = table[w][n - 1];
			}
		}
	}

	// print final table - for debugging?
	// do not delete this code
	if (num_items + W < 50)
	{ // print only if table is not too big
		std::cout << "   ";
		for (int n = 0; n <= num_items; ++n)
		{
			std::cout << n << "     ";
		}
		std::cout << "  items\n        ";
		for (int n = 0; n < num_items; ++n)
		{
			std::cout << items[n].weight << "," << items[n].value << "   ";
		}
		std::cout << "\n   ";
		for (int n = 0; n <= num_items; ++n)
		{
			std::cout << "------";
		}
		std::cout << std::endl;

		for (int w = 0; w <= W; ++w)
		{
			std::cout << w << "| ";
			for (int n = 0; n <= num_items; ++n)
			{
				std::cout << table[w][n] << "     ";
			}
			std::cout << std::endl;
		}
	}
	// end do not delete this code

	// figure out which items are in the bag based on the table
	std::vector<int> bag;
	int w = W;
	int n = num_items;

	while (w > 0 && n > 0)
	{
		if (table[w][n] != table[w][n - 1])
		{
			bag.push_back(n - 1);
			w -= items[n - 1].weight;
		}
		n--;
	}

	return bag;
}

////////////////////////////////////////////////////////////
int valueBag(std::vector<Item> const &items, std::vector<int> const &bag)
{
	std::vector<int>::const_iterator it = bag.begin(),
									 it_e = bag.end();

	int accum = 0;
	// std::cout << "Bag ";
	for (; it != it_e; ++it)
	{
		accum += items[*it].value;
		// std::cout << *it << " ";
	}
	// std::cout << std::endl;
	return accum;
}

////////////////////////////////////////////////////////////
// prototype
// notice that auxiliary function returns value of the vector of items
// the actual vector is determined later from the table (similar to DP solution)
int knapsackRecMemAux(std::vector<Item> const &, int const &, int, Table &);

////////////////////////////////////////////////////////////
// function to kick start
std::vector<int> knapsackRecMem(std::vector<Item> const &items, int const &W)
{
	int num_items = items.size();
	std::vector<std::vector<int>> table(W + 1, std::vector<int>(num_items + 1, -1));

	for (int w = 0; w <= W; ++w)
	{
		table[w][0] = 0;
	}

	for (int n = 0; n <= num_items; ++n)
	{
		table[0][n] = 0;
	}

	knapsackRecMemAux(items, W, num_items, table);

	// print table - debugging?
	// do not delete this code
	if (num_items + W < 50)
	{ // print only if table is not too big
		std::cout << "   ";
		for (int n = 0; n <= num_items; ++n)
		{
			std::cout << n << "     ";
		}
		std::cout << "  items\n        ";
		for (int n = 0; n < num_items; ++n)
		{
			std::cout << items[n].weight << "," << items[n].value << "   ";
		}
		std::cout << "\n   ";
		for (int n = 0; n <= num_items; ++n)
		{
			std::cout << "------";
		}
		std::cout << std::endl;

		for (int w = 0; w <= W; ++w)
		{
			std::cout << w << "| ";
			for (int n = 0; n <= num_items; ++n)
			{
				std::cout << table[w][n] << "     ";
			}
			std::cout << std::endl;
		}
	}
	// end do not delete this code

	// figure out which items are in the bag based on the table
	std::vector<int> bag;
	int index = num_items;
	int remainingCapacity = W;

	while (index > 0 && remainingCapacity > 0)
	{
		if (table[remainingCapacity][index] != table[remainingCapacity][index - 1])
		{
			bag.push_back(index - 1);
			remainingCapacity -= items[index - 1].weight;
		}
		index--;
	}

	return bag;
}

////////////////////////////////////////////////////////////
// the real recursive function
int knapsackRecMemAux(std::vector<Item> const &items, int const &W, int index, Table &table)
{
	// base case no items or no capacity
	if (index == 0 || W == 0)
	{
		return 0;
	}

	// value is already calculated
	if (table[W][index] != -1)
	{
		return table[W][index];
	}

	// weight exceeds capacity
	if (items[index - 1].weight > W)
	{
		table[W][index] = knapsackRecMemAux(items, W, index - 1, table);
		return table[W][index];
	}
	
	int include = items[index - 1].value + knapsackRecMemAux(items, W - items[index - 1].weight, index - 1, table);
	int exclude = knapsackRecMemAux(items, W, index - 1, table);

	// update table with max
	table[W][index] = std::max(include, exclude);

	return table[W][index];
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
