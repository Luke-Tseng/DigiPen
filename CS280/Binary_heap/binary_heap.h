#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <vector>
#include <tuple>
#include <iostream> // debugging only

// key   - binary heap
// value - actual data

// forward declaration for 1-1 operator<<
template <typename T_key, typename T_value>
class BinaryHeap;

template <typename T_key, typename T_value>
std::ostream &operator<<(std::ostream &os, BinaryHeap<T_key, T_value> const &bh);

template <typename T_key, typename T_value>
class BinaryHeap
{
private:
    // indexing starts at 0 - i.e. minimum element is at index 0
    // modify left/right/parent implementations to support that
    std::vector<std::tuple<T_key, T_value>> data = {};

public:
    BinaryHeap() = default;
    BinaryHeap(std::vector<std::tuple<T_key, T_value>> &&data2) : data(std::move(data2))
    {
        init_heap();
    }
    BinaryHeap(BinaryHeap const &) = default;            // vector copy is OK
    BinaryHeap(BinaryHeap &&) = default;                 // vector move is OK
    BinaryHeap &operator=(BinaryHeap const &) = default; // vector assignment is OK
    BinaryHeap &operator=(BinaryHeap &&) = default;      // vector move assignment is OK

    friend std::ostream &operator<<(std::ostream &os, BinaryHeap<T_key, T_value> const &bh)
    {
        for (std::tuple<T_key, T_value> const &el : bh.data)
        {
            os << std::get<0>(el) << " "; // does not print values
        }
        return os;
    }

    void push(std::tuple<T_key, T_value> t)
    {
        data.push_back(t);
        decrease_key(data.size() - 1, std::get<0>(t));
    }

    void push(std::tuple<T_key, T_value> &&t)
    {
        data.push_back(std::move(t));
        decrease_key(data.size() - 1, std::get<0>(data.back()));
    }

    void pop()
    {
        std::swap(data.front(), data.back());
        data.pop_back();

        if (!data.empty())
        {
            heapify(0);
        }
    }

    int size() const
    {
        return data.size();
    }

    std::tuple<T_key, T_value> const &top() const
    {
        return data.front();
    }

    void decrease_key(int index, T_key new_key) // bubble up
    {
        std::get<0>(data[index]) = new_key;
        while (index > 0 && std::get<0>(data[index]) < std::get<0>(data[parent(index)]))
        {
            std::swap(data[index], data[parent(index)]);
            index = parent(index);
        }
    }

private:
    int left(int i) const { return 2 * i + 1; }
    int right(int i) const { return 2 * i + 2; }
    int parent(int i) const { return (i - 1) / 2; }

    void init_heap()
    {
        for (int i = parent(size() - 1); i >= 0; --i)
        {
            heapify(i);
        }
    }

    void heapify(int index) // trickle down
                            // assume left(index) and right(index) are heaps
                            // and overall structure is heap
    {
        int smallest = index;
        int l = left(index);
        int r = right(index);

        if (l < size() && std::get<0>(data[l]) < std::get<0>(data[smallest]))
        {
            smallest = l;
        }
        if (r < size() && std::get<0>(data[r]) < std::get<0>(data[smallest]))
        {
            smallest = r;
        }

        if (smallest != index)
        {
            std::swap(data[index], data[smallest]);
            heapify(smallest);
        }
    }
};

#endif
