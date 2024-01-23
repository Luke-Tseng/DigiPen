#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <iostream>

template <typename T, int N> class Polynomial;

template <typename T, int N>
std::ostream &operator<<(std::ostream &out, Polynomial<T, N> const &pol);

template <typename T, int N>
class Polynomial
{

public:
	T coeff[N + 1];
	Polynomial();
	template <typename T2>
	Polynomial(const Polynomial<T2,N> &p);
	template<typename T2>
	Polynomial& operator=(const Polynomial<T2,N> &p);
	T operator()(T a) const;
	T &operator[](int i);
	T operator[](int i) const;
	template<int M>
	Polynomial<T, N+M> operator*(Polynomial<T, M> const& p) const;

	friend std::ostream &operator<< <T,N>(std::ostream &out, Polynomial<T, N> const &pol);

};

#include "polynomial.cpp"
#endif
