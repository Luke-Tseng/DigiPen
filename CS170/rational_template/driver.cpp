#include "rational.h"
#include <iostream>
#include <iomanip> // setprecision


void test0()
{
    CS170::Rational<int> r1(1,2);
    std::cout << r1 << std::endl;

    CS170::Rational<int> r2(-1,2);
    std::cout << r2 << std::endl;

    CS170::Rational<int> r3(1,-2);
    std::cout << r3 << std::endl;
}

void test1()
{
    CS170::Rational<int> r1(4,8);
    std::cout << r1 << std::endl;

    CS170::Rational<int> r2(-6,8);
    std::cout << r2 << std::endl;

    CS170::Rational<int> const r3(3,-9);
    std::cout << r3 << std::endl;
}

void test2()
{
    CS170::Rational<int> r1(6);
    std::cout << r1 << std::endl;

    CS170::Rational<int> r2(-3);
    std::cout << r2 << std::endl;
}

void test3()
{
    CS170::Rational<int> r1(4,8);

    CS170::Rational<int> const r2( r1 );

    CS170::Rational<int> r3(3,-9);
    std::cout << r1 << std::endl;
    std::cout << r2 << std::endl;
    std::cout << r3 << std::endl;

    r3 = r2;
    std::cout << r1 << std::endl;
    std::cout << r2 << std::endl;
    std::cout << r3 << std::endl;
}

void test4()
{
    CS170::Rational<int> const pi_1( 16, 5 );     // approximation of pi (see "Indiana Pi Bill" of 1897)
    CS170::Rational<int> const pi_2( 22, 7 );     // approximation of pi (Ancient Egypt)
    CS170::Rational<int> const pi_3( 355, 113 );  // approximation of pi (Milu approximation by Chiniese astronomer Zu Chongzhi)
    
    // pi_1 > pi_2 > pi_3

    std::cout << "pi_1 and pi_2 are " << (pi_1 != pi_2 ? "not ":"") << "equal\n";
    std::cout << "pi_2 and pi_3 are " << (pi_2 != pi_3 ? "not ":"") << "equal\n";
    std::cout << "pi_1 and pi_3 are " << (pi_1 != pi_3 ? "not ":"") << "equal\n";
    std::cout << "pi_1 and pi_2 are " << (pi_1 == pi_2 ? "":"not ") << "equal\n";
    std::cout << "pi_2 and pi_3 are " << (pi_2 == pi_3 ? "":"not ") << "equal\n";
    std::cout << "pi_1 and pi_3 are " << (pi_1 == pi_3 ? "":"not ") << "equal\n";

    std::cout << "pi_1 is " << (pi_1 < pi_2 ? "":"not ") << "less than pi_2\n";
    std::cout << "pi_1 is " << (pi_1 < pi_3 ? "":"not ") << "less than pi_3\n";
    std::cout << "pi_2 is " << (pi_2 < pi_3 ? "":"not ") << "less than pi_3\n";

    std::cout << "pi_1 is " << (pi_1 > pi_2 ? "":"not ") << "greater than pi_2\n";
    std::cout << "pi_1 is " << (pi_1 > pi_3 ? "":"not ") << "greater than pi_3\n";
    std::cout << "pi_2 is " << (pi_2 > pi_3 ? "":"not ") << "greater than pi_3\n";

    std::cout << "pi_1 is " << (pi_1 <= pi_2 ? "":"not ") << "less or equal than pi_2\n";
    std::cout << "pi_1 is " << (pi_1 <= pi_3 ? "":"not ") << "less or equal than pi_3\n";
    std::cout << "pi_2 is " << (pi_2 <= pi_3 ? "":"not ") << "less or equal than pi_3\n";

    std::cout << "pi_1 is " << (pi_1 >= pi_2 ? "":"not ") << "greater or equal than pi_2\n";
    std::cout << "pi_1 is " << (pi_1 >= pi_3 ? "":"not ") << "greater or equal than pi_3\n";
    std::cout << "pi_2 is " << (pi_2 >= pi_3 ? "":"not ") << "greater or equal than pi_3\n";
}

void test5()
{
    CS170::Rational<int> const r1( -1,2 );
    CS170::Rational<int> const r2( 1,-2 );
    CS170::Rational<int> const r3( 10,-20 );

    std::cout << "r1 and r2 are " << (r1 == r2 ? "":"not ") << "equal\n";
    std::cout << "r2 and r3 are " << (r2 == r3 ? "":"not ") << "equal\n";
    std::cout << "r1 and r3 are " << (r1 == r3 ? "":"not ") << "equal\n";
}

void test6()
{
    CS170::Rational<int> const one_sixth( 1,6 );
    CS170::Rational<int> r; // default = zero (0/1)

    for( int i=0; i<60; ++ i ) {
        r += one_sixth;
        std::cout << r << std::endl;
    }
    std::cout << std::endl;
}

void test7()
{
    CS170::Rational<int> const one_sixth( 1,6 );

    for( int i=0; i<60; ++ i ) {
        CS170::Rational<int> r( i );
        r *= one_sixth;
        std::cout << r << std::endl;
    }
    std::cout << std::endl;
}

void test8()
{
    CS170::Rational<int> const one_sixth( 1,6 );

    for( int i=0; i<60; ++ i ) {
        CS170::Rational<int> r( one_sixth );
        r *= i;                     // conversion int -> CS170::Rational<int> in the second argument will be done by compiler by calling CS170::Rational<int>(int)
        std::cout << r << std::endl;
    }
    std::cout << std::endl;
}

void test9()
{
    CS170::Rational<long> const one_sixth( 1,6 );
    CS170::Rational<long> r( -one_sixth );   // -1/6, requires unary negation operator

    for( int i=0; i<20; ++ i ) {
        CS170::Rational<long> m( one_sixth );
        std::cout << m*i << " ";
        std::cout << (r+one_sixth) << " "; // try removing parentheses 
        r = r + one_sixth; 
        std::cout << std::endl;
    }
}

void test10()
{
  // same as test6, but more itertions and using long - may allow more digits
    CS170::Rational<long> const one_sixth( 1,6 );
    CS170::Rational<long> r; // default = zero (0/1)

    for( int i=0; i<20; ++ i ) {
        r += one_sixth;
        std::cout << r << std::endl;
    }
    std::cout << std::endl;
}

void test11()
{
    // Leibniz formula for pi
    // pi = 4/1 - 4/3 + 4/5 - 4/7 + 4/9 - 4/11 ...
    // very slow convergence
    int len = 10;
    CS170::Rational<int> pi; // 0/1 by default
    for ( int i=0; i<len; ++i ) {
        pi += (i%2 ? CS170::Rational<int>(-4) : CS170::Rational<int>(4) ) / (2*i+1);
        std::cout << pi << " appr " << std::setprecision(8) << static_cast<double>( pi.numerator() ) / pi.denominator() << std::endl;
    }
}

void test12()
{
    // John Wallis formula for pi
    //         inf
    //         +--+  4* i^2
    // pi = 2 *|  | ---------  (product)
    //         |  | 4* i^2 -1
    //         n=1
    int len = 7; 
    CS170::Rational<long> pi(2);
    for ( int i=1; i<=len; ++i ) {
        pi *= CS170::Rational<long>( 4*i*i,4*i*i-1 );
        std::cout << pi << " appr " << std::setprecision(8) << static_cast<double>( pi.numerator() ) / pi.denominator() << std::endl;
    }
}

void (*pTests[])() = { test0,test1,test2,test3,test4,test5,test6,test7,test8,test9,test10,test11,test12
};

#include <cstdio>
int main (int argc, char ** argv) {
    pTests[6]();
    return 0;
}
