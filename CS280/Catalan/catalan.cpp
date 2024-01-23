#include "catalan.h"

int catalan2(int size)
{
    if (size < 2)
        return 1;

    int n = 0;
    for (int s = 0; s < size; ++s)
    {
        //    left           right    (-1 is for root)
        //    subtree        subtree
        n += (catalan2(s) * catalan2(size - 1 - s));
    }
    return n;
}

int catalan3(int size)
{
    if (size < 2)
        return 1;

    int n = 0;
    for (int s = 0; s < size; ++s)
    {
        for (int t = 0; t < size - s; ++t)
        {
            n += (catalan3(s) * catalan3(t) * catalan3(size - s - t - 1));
        }
    }
    return n;
}

int catalan4(int size)
{
    if (size < 2)
        return 1;

    int n = 0;
    for (int s = 0; s < size; ++s)
    {
        for (int t = 0; t < size - s; ++t)
        {
            for (int u = 0; u < size - s - t; ++u)
            {
                n += (catalan4(s) * catalan4(t) * catalan4(u) * catalan4(size - s - t - u - 1));
            }
        }
    }
    return n;
}

int catalan(int size, int b)
{
    if (size < 2)
        return 1;

    int n = 0;
    for (int s = 0; s < size; ++s)
    {
        int sum = 1;
        for (int i = 0; i < b; ++i)
        {
            int j = s + i * (size - s) / b + 1;
            sum *= catalan(size - j, b);
        }
        n += (catalan(s, b - 1) * sum);
    }
    return n;
}