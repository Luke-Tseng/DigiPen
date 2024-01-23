#ifndef STATISTICS_H
#define STATISTICS_H
#include <vector>    //vector
#include <algorithm> //copy
#include <ostream>   //ostream
#include <istream>   //istream
#include <map>       //map

class Statistics
{
private:
    typedef std::vector<int> ContainerType;
    ContainerType data;

public:
    Statistics();
    template <typename T>
    Statistics(T const *b, T const *e);
    double Average() const;
    int Size() const;
    int Minimum() const;
    int Maximum() const;
    double Deviation() const;
    std::vector<int> Histogram(int bins, int min, int max) const;
    std::map<int, int> OccuresMoreThan(int n) const;
    template <typename F>
    void RemoveIf(F p);
    void DrawHistogramH(int bins, int min, int max) const;
    void DrawHistogramV(int bins, int min, int max) const;

    friend std::ostream &operator<<(std::ostream &out, Statistics const &stat);
    friend std::istream &operator>>(std::istream &in, Statistics &stat);
};

template <typename T>
Statistics::Statistics(T const *b, T const *e) : data(e - b)
{
    std::copy(b, e, data.begin()); // we can use regular pointers to specify ranges!
}

template <typename F>
void Statistics::RemoveIf(F p)
{
    ContainerType::iterator it = data.begin();
    while (it != data.end())
    {
        if (p(*it))
        {
            it = data.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

#endif
