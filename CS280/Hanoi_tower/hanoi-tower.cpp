#include <sstream> // std::stringstream
#include <iomanip> // left,right,fill,setw - see handout

template <typename CALLBACK>
void hanoi_tower_1_rec(int n, char pegA, char pegB, char pegC, CALLBACK cb)
{
    if (n == 1)
    {
        std::stringstream ss;
        ss << "move disk 1 from " << pegA << " to " << pegC;
        cb(ss.str());
        return;
    }

    hanoi_tower_1_rec(n - 1, pegA, pegC, pegB, cb);
    std::stringstream ss;
    ss << "move disk " << n << " from " << pegA << " to " << pegC;
    cb(ss.str());
    hanoi_tower_1_rec(n - 1, pegB, pegA, pegC, cb);
}

template <typename CALLBACK>
void hanoi_tower_1(int n, CALLBACK cb)
{
    hanoi_tower_1_rec(n, 'A', 'B', 'C', cb);
}

////////////////////////////////////////////////////////////////////////////////
template <typename CALLBACK>
void hanoi_tower_2(int n, CALLBACK cb)
{
    hanoi_tower_1(n, cb); // if not doing extra credit just leave it like this
}

////////////////////////////////////////////////////////////////////////////////
template <typename CALLBACK>
void hanoi_tower_3(int n, CALLBACK cb)
{
    hanoi_tower_1(n, cb); // if not doing extra credit just leave it like this
}
