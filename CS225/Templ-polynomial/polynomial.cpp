#include "polynomial.h"
#include <math.h>

template <typename T, int N>
Polynomial<T, N>::Polynomial()
{
    for(int i = 0;i <= N;i++)
    {
        coeff[i] = 0;
    }
}

template <typename T, int N>
template <typename T2>
Polynomial<T, N>::Polynomial(const Polynomial<T2, N> &p)
{
    for (int i = 0; i <= N; i++)
    {
        coeff[i] = static_cast<T>(p.coeff[i]);
    }
}

template <typename T, int N>
template <typename T2>
Polynomial<T, N> &Polynomial<T, N>::operator=(const Polynomial<T2, N> &p)
{
    for (int i = 0; i <= N; i++)
    {
        coeff[i] = static_cast<T>(p.coeff[i]);
    }
    return *this;   
}

template <typename T, int N>
T &Polynomial<T, N>::operator[](int i)
{
    return coeff[i];
}

template <typename T, int N>
T Polynomial<T, N>::operator[](int i) const
{
    return coeff[i];
}

template <typename T, int N>
T Polynomial<T, N>::operator()(T a) const
{
    T total = 0;
    
    for (int i = 0; i <= N; i++)
    {
        total += pow(a, i) * coeff[i];  
    }
    return total;
}

template <typename T, int N>
template <int M>
Polynomial<T, N+M> Polynomial<T,N>::operator*(Polynomial<T, M> const& p) const
{
    Polynomial<T, N+M> result;
    for (int i = 0; i <= N; i++)
    {
        for (int j = 0; j <= M; j++)
        {
            result.coeff[i + j] += coeff[i] * p.coeff[j];
        }
    }
    return result;
}

// do not change this function
template <typename T, int N>
std::ostream &operator<<(std::ostream &out, Polynomial<T, N> const &pol)
{
    out << pol.coeff[0] << " ";
    for (int i = 1; i <= N; ++i)
    {
        if (pol.coeff[i] != 0)
        { // skip terms with zero coefficients
            if (pol.coeff[i] > 0)
            {
                out << "+";
            }

            if (pol.coeff[i] == 1)
            {
            }
            else if (pol.coeff[i] == -1)
            {
                out << "-";
            }
            else
            {
                out << pol.coeff[i] << "*";
            }
            out << "x^" << i << " ";
        }
    }
    return out;
}
