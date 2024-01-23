#include "polynomial.h"
#include <iostream>

int main( )
{   // assignment to different degree
    Polynomial<float,2> p2;
    p2[0] = 5;
    p2[1] = 5;
    p2[2] = 5;

    Polynomial<float,3> p3;
    p3 = p2;
    return 0;
}
