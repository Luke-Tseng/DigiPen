/*
2x3x4
+----------------------------------------+
|+------------------++------------------+|
||+----++----++----+||+----++----++----+||
|||1234||1234||1234||||1234||1234||1234|||
||+----++----++----+||+----++----++----+||
|+------------------++------------------+|
+----------------------------------------+

+------+
|abcdef| 6 pointers to 1's ( array of row pointers )
+------+

+--+
|mn| 2 pointers to a and d above ( array of pointers to slices )
+--+

a[i][j][k]
i chooses which slice
j chooses which row within the slice
k chooses within element with the row
*/

#include "tddaa.h"
#include <stdlib.h>

int ***allocate(int d1, int d2, int d3)
{
    int i, j;
    int ***ppp;
    int **pp;
    int *p;

    p = (int *) malloc(d1 * d2 * d3 * sizeof(int));

    pp = (int **) malloc(d1 * d2 * sizeof(int *)); 
    
    ppp = malloc(d1 * sizeof(int **));

    for (i = 0; i < d1 * d2; ++i)
        pp[i] = p + (d3 * i);

    for (j = 0; j < d1; ++j)
        ppp[j] = pp + (d2 * j);

    return ppp;
}   

void deallocate(int ***ppp)
{
    free(**ppp);
    free(*ppp);
    free(ppp);
}
