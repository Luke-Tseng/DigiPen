#include "rational.h"
#include <iomanip>  // std::setw

namespace CS170 {

    int Rational::width = 0; // no padding by default

    ////////////////////////////////////////////////////////////////////////
    Rational::Rational() : n(0), d(1) {}
    ////////////////////////////////////////////////////////////////////////
    Rational::Rational(int i) : n(i), d(1) {}
    ////////////////////////////////////////////////////////////////////////
    Rational::Rational(int _n, int _d) : n(_n), d(_d) { CheckSign(); CheckGCD(); }
    ////////////////////////////////////////////////////////////////////////
    Rational Rational::operator- () const { return Rational(-n, d); }
    ////////////////////////////////////////////////////////////////////////

    Rational& Rational::operator+= (const Rational& other) {
        n = other.d * n + other.n * d;
        d = d * other.d;
        CheckSign();
        CheckGCD();
        return *this;
    }

    Rational& Rational::operator*= (const Rational& other) {
        n *= other.n;
        d *= other.d;
        CheckSign();
        CheckGCD();
        return *this;
    }

    Rational& Rational::operator-= (const Rational& other) {
        *this += -other;
        return *this;
    }

    Rational& Rational::operator/= (const Rational& other) {
        n *= other.d;
        d *= other.n;
        CheckSign();
        CheckGCD();
        return *this;
    }

    Rational Rational::operator* (const Rational& other) {
        Rational r(n, d);
        r *= other;
        return r;
    }

    Rational Rational::operator+ (const Rational& other) {
        Rational r(n,d);
        r += other;
        return r;
    }

    Rational Rational::operator- (const Rational& other) {
        Rational r(n, d);
        r -= other;
        return r;
    }

    Rational Rational::operator/ (const Rational& other) {
        Rational r(n, d);
        r /= other;
        return r;
    }

    Rational Rational::operator+ (const Rational& other) const {
        Rational r(n, d);
        r += other;
        return r;
    }

    Rational Rational::operator- (const Rational& other) const {
        Rational r(n, d);
        r -= other;
        return r;
    }

    Rational Rational::operator/ (const Rational& other) const {
        Rational r(n, d);
        r /= other;
        return r;
    }

    ////////////////////////////////////////////////////////////////////////
    // comparisons
    bool Rational::operator< ( Rational const & op2 ) const 
    { 
        return (n*op2.d < d*op2.n); 
    }

    bool Rational::operator> (Rational const& op2) const
    {
        return (n * op2.d > d* op2.n);
    }

    bool Rational::operator<= (Rational const& op2) const
    {
        return (*this < op2 || *this == op2);
    }

    bool Rational::operator>= (Rational const& op2) const
    {
        return *this > op2 || *this == op2;
    }

    bool Rational::operator!= (Rational const& op2) const
    {
        return !(*this == op2);
    }

    bool Rational::operator== (Rational const& op2) const
    {
        return (n == op2.n && d == op2.d);
    }
    ////////////////////////////////////////////////////////////////////////
    std::ostream& operator<<( std::ostream& os, Rational const& r ) 
    {
        os << std::setw( r.width ) << r.n;
        if ( r.d != 1 ) { os << "/" << std::setw( r.width-1 ) << r.d; }
        else            { os <<        std::setw( r.width ) << " "; }
        return os;
    }
    ////////////////////////////////////////////////////////////////////////
    void Rational::CheckSign() 
    {
        if ( d<0 ) { n *= -1; d*= -1; }
    }
    ////////////////////////////////////////////////////////////////////////
    void Rational::CheckGCD() 
    {
        int gcd = GCD();
        n = n/gcd;
        d = d/gcd;
    }
    ////////////////////////////////////////////////////////////////////////
    // Euclid's algorithm
    int Rational::GCD() const 
    {
        int a=std::abs(n), b=std::abs(d);
        while ( a && b ) {
            if ( a < b ) { b = b%a; }
            else         { a = a%b; }
        }
        if ( a==0 ) return b;
        else        return a;
    }

    void Rational::Width(int w) {
        width = w;
    }
}
