#include <iostream>
#include <vector>

// actual recursive function
template <typename T>
void subset_rec(const std::vector<T> &set, std::vector<T> &subset, int index)
{
    if (index == static_cast<int>(set.size()))
    {
        std::cout << "{ ";
        for (int i = 0; i < static_cast<int>(subset.size()); i++)
        {
            std::cout << subset[i] << " ";
            if (i != static_cast<int>(subset.size() - 1))
            {
                std::cout << "";
            }
        }
        std::cout << "}" << std::endl;
        return;
    }
    // add the current element
    subset.push_back(set[index]);
    subset_rec(set, subset, index + 1);
    subset.pop_back();
    // not add the current element
    subset_rec(set, subset, index + 1);
}

// kickstart
template <typename T>
void subsets(const std::vector<T> &set)
{
    std::vector<T> subset;
    subset_rec(set, subset, 0);
}
