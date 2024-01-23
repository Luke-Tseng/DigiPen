#include "polynomial.h"
#include <iostream>

int main( )
{   // conversion
    Polynomial<float,2> pf_1;
    pf_1[0] = 1.1;
    pf_1[1] = -2.2;
    pf_1[2] = 1.1;
    std::cout << pf_1 << std::endl;

    // conversion to a different degree should NOT compile
    Polynomial<float,3> p2( pf_1 );
    return 0;
}
