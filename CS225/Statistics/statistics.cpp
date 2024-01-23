#include "statistics.h"
#include <numeric>  //accumulate
#include <cmath>    //sqrt
#include <iterator> //ostream_iterator, back_inserter
#include <iomanip>  //setfill setw
#include <iostream> //cout

Statistics::Statistics() : data() {}

double Statistics::Average() const
{
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

int Statistics::Size() const
{
    return data.size();
}

int Statistics::Minimum() const
{
    return *std::min_element(data.begin(), data.end());
}

int Statistics::Maximum() const
{
    return *std::max_element(data.begin(), data.end());
}

double Statistics::Deviation() const
{
    double avg = Average();
    double sum = 0;
    for (ContainerType::const_iterator it = data.begin(); it != data.end(); ++it)
    {
        sum += (*it - avg) * (*it - avg);
    }
    return sqrt(sum / data.size());
}

std::map<int, int> Statistics::OccuresMoreThan(int n) const
{
    std::map<int, int> result;
    for (ContainerType::const_iterator it = data.begin(); it != data.end(); ++it)
    {
        if (std::count(data.begin(), data.end(), *it) > n)
        {
            result[*it] = std::count(data.begin(), data.end(), *it);
        }
    }
    return result;
}

std::vector<int> Statistics::Histogram(int bins, int min, int max) const
{
    std::vector<int> result(bins, 0);
    for (ContainerType::const_iterator it = data.begin(); it != data.end(); ++it)
    {
        int bin = (*it - min) * bins / (max - min);
        if (bin == bins)
            --bin;
        ++result[bin];
    }
    return result;
}

void Statistics::DrawHistogramH(int bins, int min, int max) const
{
    std::vector<int> hist = Histogram(bins, min, max);
    for (int i = 0; i < bins; ++i)
    {
        std::cout << std::setfill('*') << std::setw(hist[i]) << "" << std::endl;
    }
}

void Statistics::DrawHistogramV(int bins, int min, int max) const
{
    std::vector<int> hist = Histogram(bins, min, max);
    int maxCount = *std::max_element(hist.begin(), hist.end());
    for (int i = maxCount; i > 0; --i)
    {
        for (int j = 0; j < bins; ++j)
        {
            if (hist[j] >= i)
            {
                std::cout << "*";
            }
            else
            {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

std::ostream &operator<<(std::ostream &out, Statistics const &stat)
{
    std::copy(stat.data.begin(), stat.data.end(),
              std::ostream_iterator<int>(out, " "));
    return out;
}

std::istream &operator>>(std::istream &in, Statistics &stat)
{
    std::copy(std::istream_iterator<int>(in),
              std::istream_iterator<int>(),
              std::back_inserter(stat.data));
    return in;
}
