#include <iostream>     // cout, endl
#include <random>
#include <algorithm>    // copy, random_shuffle
#include <iterator>     // ostream_iterator
#include "bubble-sort.h"
// all 3 containers support push_front
#include <deque>        // random access
#include <list>         // bidirectional
#include <forward_list> // forward only

struct Yaint {
    int i;
    Yaint( int i =0 ) : i(i) {}
    bool operator<( Yaint const& arg2 ) const {
        ++num_comparisons;
        return i < arg2.i;
    }
    static int num_comparisons;

};
int Yaint::num_comparisons = 0;

void random_array( size_t size, int num_experiments )
{
    size_t * a = new size_t[size];
    for ( size_t i=0; i<size; ++i ) { a[i] = i; }

    //std::copy(a, a + size, std::ostream_iterator<int>(std::cout, " "));
    //std::cout << std::endl;

    bool OK = true;
    for ( int i=0; i<num_experiments and OK; ++i ) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle( a, a+size, g );

//        std::cout << "before: ";
//        std::copy(a, a + size, std::ostream_iterator<int>(std::cout, " "));
//        std::cout << std::endl;

        bubblesort( a, a+size );


        //check result
        for ( size_t i=0; i<size; ++i ) {
            if ( a[i] != i ) {
                std::cout << "Error at index " << i << std::endl;
                OK = false;
            }
        }
    }

    if ( OK ) {
        std::cout << "OK" << std::endl;
    }

    delete [] a;
}

template< typename ContainerType >
void generate_random( typename ContainerType::size_type size )
{
    using stype = typename ContainerType::size_type;
    int * a = new int[size];
    for ( stype i=0; i<size; ++i ) { a[i] = i; }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle( a, a+size, g );

    ContainerType c;
    for ( stype i=0; i<size; ++i ) { 
        c.push_front( a[i] );
    }

    bubblesort( c.begin(), c.end() );

    //std::copy(a, a + n, std::ostream_iterator<int>(std::cout, " "));
    //std::cout << std::endl;

    //check result
    int i=0;
    bool OK = true;
    for ( auto const& el : c ) {
        if ( el != i ) {
            std::cout << "Error at index " << i << std::endl;
            OK = false;
        }
        ++i;
    }
    
    if ( OK ) {
        std::cout << "OK" << std::endl;
    }

    delete [] a;
}

void test0() { random_array( 10,    1 ); }  // debugging
void test1() { random_array( 100,  10 ); }
void test2() { random_array( 10000,10 ); } // stress test ( 1.6 sec on AMD FX-8320)
void test3() { 
    generate_random< std::deque<int> >( 100 );  // provides random access iterator
}
void test4() { 
    generate_random< std::list<int> >( 100 );   // provides bidirectional iterator
}
void test5() { 
    generate_random< std::forward_list<int> >( 100 );   // provides forward iterator only
}

void test6() {}  
void test6a() { 
    // recheck correctness
    test0();
    test1();

    // check efficiency
    int size = 10;
    Yaint * a = new Yaint[size];
    for ( int i=0; i<size; ++i ) { a[i] = i; } // create sorted         
    std::reverse( a, a+5 );                 // reverse first 5 elements           
    //std::reverse( a+(size-5), a+size );   // reverse last 5 elements
    //std::reverse( a, a+size );            // reverse the whole array (worst case)

    //                      BS1    BS2    BS3   <- possible implementations of Bubble sort
    //  first 5 reversed    45     35     15
    //  last 5 reversed     45     35     35
    //  all 5 reversed      90     45     45
    //std::copy(a, a + size, std::ostream_iterator<int>(std::cout, " "));
    //std::cout << std::endl;


    bubblesort( a, a+size );
    std::cout << "Number of comparisons performed: " << Yaint::num_comparisons << std::endl;

    delete [] a;
}

void (*pTests[])(void) = { 
	test0,test1,test2,test3,test4,test5,test6
};

#include <cstdio> //sscanf
int main(int argc, char ** argv) {
    if (argc >1) {
        int test = 0;
        std::sscanf(argv[1],"%i",&test);
        try {
            pTests[test]();
        } catch( const char* msg) {
            std::cerr << msg << std::endl;
        }
    }

    return 0;
}
