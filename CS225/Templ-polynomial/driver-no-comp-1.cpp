#include "polynomial.h"
#include <iostream>

int main( )
{   // multiplication
    Polynomial<int,3> p3;
    p3[0] = 1;
    p3[1] = 1;
    p3[2] = 1;
    p3[3] = 1; // defines 1+x+x^2+x^3

    Polynomial<float,2> pf_1;
    pf_1[0] = 1.1;
    pf_1[1] = -2.2;
    pf_1[2] = 1.1;
    std::cout << pf_1 << std::endl;

    // multiplcation of 2 different types of coefficients 
    // should NOT compile
    pf_1 * p3;
    return 0;
}
