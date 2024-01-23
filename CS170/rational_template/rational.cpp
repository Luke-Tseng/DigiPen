#ifndef RATIONAL_CPP
#define RATIONAL_CPP
#include "rational.h"
#include <iomanip>  // setw
#include <cstdlib>  // abs

namespace CS170 {

    template < typename T> 
    int Rational<T>::width = 0; // no padding by default

    ////////////////////////////////////////////////////////////////////////
    template < typename T> 
    Rational<T>::Rational()               : n(0),  d(1)   {}
    ////////////////////////////////////////////////////////////////////////
    template < typename T> 
    Rational<T>::Rational( T i )        : n(i),  d(1)   {}
    ////////////////////////////////////////////////////////////////////////
    template < typename T> 
    Rational<T>::Rational(T _n, T _d) : n(_n), d(_d)  { CheckSign(); CheckGCD(); }

    template < typename T>
    Rational<T> Rational<T>::operator- () const { return Rational<T>(-n, d); }

    // ....
    template < typename T>
    Rational<T>& Rational<T>::operator+= (const Rational<T>& other) {
        n = other.d * n + other.n * d;
        d = d * other.d;
        CheckSign();
        CheckGCD();
        return *this;
    }

    template < typename T>
    Rational<T>& Rational<T>::operator*= (const Rational<T>& other) {
        n *= other.n;
        d *= other.d;
        CheckSign();
        CheckGCD();
        return *this;
    }

    template < typename T>
    Rational<T>& Rational<T>::operator-= (const Rational<T>& other) {
        *this += -other;
        return *this;
    }

    template < typename T>
    Rational<T>& Rational<T>::operator/= (const Rational<T>& other) {
        n *= other.d;
        d *= other.n;
        CheckSign();
        CheckGCD();
        return *this;
    }

    template < typename T>
    Rational<T> Rational<T>::operator* (const Rational<T>& other) {
        Rational r(n, d);
        r *= other;
        return r;
    }

    template < typename T>
    Rational<T> Rational<T>::operator+ (const Rational<T>& other) {
        Rational r(n, d);
        r += other;
        return r;
    }

    template < typename T>
    Rational<T> Rational<T>::operator- (const Rational<T>& other) {
        Rational r(n, d);
        r -= other;
        return r;
    }

    template < typename T>
    Rational<T> Rational<T>::operator/ (const Rational<T>& other) {
        Rational r(n, d);
        r /= other;
        return r;
    }

    template < typename T>
    Rational<T> Rational<T>::operator+ (const Rational<T>& other) const {
        Rational r(n, d);
        r += other;
        return r;
    }

    template < typename T>
    Rational<T> Rational<T>::operator- (const Rational<T>& other) const {
        Rational<T> r(n, d);
        r -= other;
        return r;
    }

    template < typename T>
    Rational<T> Rational<T>::operator/ (const Rational<T>& other) const {
        Rational<T> r(n, d);
        r /= other;
        return r;
    }

    template < typename T>
    bool Rational<T>::operator< (Rational<T> const& op2) const
    {
        return (n * op2.d < d* op2.n);
    }

    template < typename T>
    bool Rational<T>::operator> (Rational<T> const& op2) const
    {
        return (n * op2.d > d * op2.n);
    }

    template < typename T>
    bool Rational<T>::operator<= (Rational<T> const& op2) const
    {
        return (*this < op2 || *this == op2);
    }

    template < typename T>
    bool Rational<T>::operator>= (Rational<T> const& op2) const
    {
        return *this > op2 || *this == op2;
    }

    template < typename T>
    bool Rational<T>::operator!= (Rational<T> const& op2) const
    {
        return !(*this == op2);
    }

    template < typename T>
    bool Rational<T>::operator== (Rational<T> const& op2) const
    {
        return (n == op2.n && d == op2.d);
    }

    ////////////////////////////////////////////////////////////////////////
    template < typename U> 
    std::ostream& operator<<( std::ostream& os, Rational<U> const& r ) {
        os << std::setw( r.width ) << r.n;
        if ( r.d != 1 ) { os << "/" << std::setw( r.width-1 ) << r.d; }
        else            { os <<        std::setw( r.width ) << " "; }
        return os;
    }
    ////////////////////////////////////////////////////////////////////////
    template < typename T> 
    void Rational<T>::CheckSign() {
        if ( d<0 ) { n *= -1; d*= -1; }
    }
    ////////////////////////////////////////////////////////////////////////
    template < typename T> 
    void Rational<T>::CheckGCD() {
        int gcd = GCD();
        n = n/gcd;
        d = d/gcd;
    }
    ////////////////////////////////////////////////////////////////////////
    template < typename T> 
    int Rational<T>::GCD() const {
        int a=std::abs(n), b=std::abs(d);
        while ( a && b ) {
            if ( a < b ) { b = b%a; }
            else         { a = a%b; }
        }
        if ( a==0 ) return b;
        else        return a;
    }
}

#endif