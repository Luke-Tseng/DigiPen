#include "polynomial.h"
#include <iostream>

void test0()
{   // default ctor and index
    std::cout << "-------- " << __func__ << " --------\n";
    Polynomial<int,3> p3;
    std::cout << p3 << std::endl; // all zeroes

    p3[0] = 1;
    p3[1] = 1;
    p3[2] = 1;
    p3[3] = 1; 
    std::cout << p3 << std::endl; // all ones

    Polynomial<float,1> p1;
    p1[0] = 2.2;
    p1[1] = 10.1; 
    std::cout << p1 << std::endl;
}

void test1()
{   // multiplication
    std::cout << "-------- " << __func__ << " --------\n";
    Polynomial<int,3> p3;
    p3[0] = 1;
    p3[1] = 1;
    p3[2] = 1;
    p3[3] = 1; // defines 1+x+x^2+x^3

    Polynomial<int,1> p1;
    p1[0] = 1;
    p1[1] = -1; // defines 1-x
    std::cout << p1 << std::endl;

    Polynomial<int,4> p4 = p3*p1;
    std::cout << p4 << std::endl; // produces 1+ -1 * x^4
                                  // (1+x+x^2+x^3)*(1-x) = 1-x^4

    Polynomial<float,2> pf_1;
    pf_1[0] = 1.1;
    pf_1[1] = -2.2;
    pf_1[2] = 1.1;
    std::cout << pf_1 << std::endl;

    // "make gcc1_NC" see driver-no-comp-1.cpp
    // multiplcation of 2 different types of coefficients 
    // should NOT compile
    // pf_1 * p3;
}

void test2()
{   // conversion
    std::cout << "-------- " << __func__ << " --------\n";
    Polynomial<float,2> pf_1;
    pf_1[0] = 1.1;
    pf_1[1] = -2.2;
    pf_1[2] = 1.1;
    std::cout << pf_1 << std::endl;

    Polynomial<int,2> p2( pf_1 ); // convert
    std::cout << p2 << std::endl;

    // "make gcc2_NC" see driver-no-comp-2.cpp
    // conversion to a different degree should NOT compile
    // Polynomial<int,3> p2( pf_1 );
} 

void test3()
{   // copy 
    std::cout << "-------- " << __func__ << " --------\n";
    // note that compiler will generate copy-ctor
    // templated conversion ctor will NOT be used
    // compiler generated copy is OK - no dynamically allocated data.
    Polynomial<int,2> p2;
    p2[0] = 3;
    p2[1] = 2;
    p2[2] = 1;
    std::cout << p2 << std::endl;
    
    Polynomial<int,2> p2_copy( p2 );
    std::cout << p2_copy << std::endl;

//    // same for assignment
//    // using templated 
//    pf_1 = p2; // float = int

    p2[0] = 5;
    p2[1] = 5;
    p2[2] = 5;
    // using compiler generated assignment
    p2_copy = p2;
    std::cout << p2_copy << std::endl;
}

void test4()
{   // templated assignment
    std::cout << "-------- " << __func__ << " --------\n";
    
    Polynomial<float,2> pf_2;
    pf_2[0] = 1.1;
    pf_2[1] = -2.2;
    pf_2[2] = 1.1;
    std::cout << pf_2 << std::endl;

    Polynomial<int,2> p2;
    p2[0] = 3;
    p2[1] = 2;
    p2[2] = 1;
    std::cout << p2 << std::endl;

    p2 = pf_2;
    std::cout << p2 << std::endl;
}

void test5()
{   // evaluate
    std::cout << "-------- " << __func__ << " --------\n";
    
    Polynomial<int,2> p2;
    p2[0] = 1;
    p2[1] = -2;
    p2[2] = 1;
    std::cout << p2 << std::endl;

    Polynomial<int,5> p5;
    p5[0] = 3;
    p5[1] = -2;
    p5[2] = 1;
    p5[3] = 1;
    p5[4] = -1;
    p5[5] = 1;
    std::cout << p5 << std::endl;
    
    Polynomial<int,7> p7_1 = p2*p5;
    Polynomial<int,7> p7_2 = p5*p2; // should be commutative
    std::cout << p7_1 << std::endl;
    int values[] = { 1,2,3,4,5 };
    for ( int i=0; i<5; ++i ) {
        int r1 = p2(values[i]);
        int r2 = p5(values[i]);
        int r3 = p7_1(values[i]);
        int r4 = p7_2(values[i]);
        std::cout << r3 << " " << r4 << std::endl;
        if ( r1*r2 != r3 or r1*r2 != r4 ) {
            std::cout << "Error\n";
        }
    }
}

#define cplus_plus_03
//#define cplus_plus_11

#ifdef cplus_plus_03
#include <cstdlib>  
#include <ctime>
#endif

#ifdef cplus_plus_11
#include <random> // C++11
#endif

void test6()
{   // evaluate randomly generated polynomials
    std::cout << "-------- " << __func__ << " --------\n";

#ifdef cplus_plus_11
    std::random_device          rd;
    std::mt19937                gen( rd() );
    std::uniform_int_distribution<>  rand_coeff( -10, 10 );
#endif

#ifdef cplus_plus_03
    std::srand( std::time(NULL) );
#endif

    
    Polynomial<int,5> p5;
    for ( int i=0; i<=5; ++i ) { 
#ifdef cplus_plus_11
        p5[i] = rand_coeff( gen ); 
#endif
#ifdef cplus_plus_03
        p5[i] = ( std::rand() %20 ) -10 ; // -10..9
#endif
    }

    Polynomial<int,4> p4;
    for ( int i=0; i<=4; ++i ) { 
#ifdef cplus_plus_11
        p4[i] = rand_coeff( gen ); 
#endif
#ifdef cplus_plus_03
        p4[i] = ( std::rand() %20 ) -10 ; // -10..9
#endif
    }
    
    Polynomial<int,9> p9_1 = p4*p5;
    Polynomial<int,9> p9_2 = p5*p4; // should be commutative

    int values[] = { 1,2,3,4,5 };
    for ( int i=0; i<5; ++i ) {
        int r1 = p4(values[i]);
        int r2 = p5(values[i]);
        int r3 = p9_1(values[i]);
        int r4 = p9_2(values[i]);
        if ( r1*r2 != r3 or r1*r2 != r4 ) {
            std::cout << "Error\n";
        } else {
            std::cout << "OK\n";
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void (*pTests[])(void) = { 
    test0, test1, test2, test3, test4, test5, test6
};

void test_all() {
    for (size_t i = 0; i<sizeof(pTests)/sizeof(pTests[0]); ++i)
        pTests[i]();
}


#include <cstdio> /* sscanf */
int main(int argc, char *argv[] ) {
    if (argc >1) {
        int test = 0;
        std::sscanf(argv[1],"%i",&test);
        try {
            pTests[test]();
        } catch( const char* msg) {
            std::cerr << msg << std::endl;
        }
    } else {
        try {
            test_all();
        } catch( const char* msg) {
            std::cerr << msg << std::endl;
        }
    }


    return 0;
}
