#include <stdio.h>

void print_cubes(void)
{
int digit;
for(digit = 100;digit<=999;++digit)
{

    int first = digit / 100;
    int second = digit / 10 % 10;
    int third = digit % 10;
    int total;
    first = first*first*first;
    second = second*second*second;
    third = third*third*third;
    total = first + second + third;
    if(total == digit)
        printf("%d has the cube property. (%d + %d + %d)\n",
        digit,first,second,third);
}
}


void print_pythagorean_triples(int low, int high) 
{
    int i,j,k;
    int count = 1;
    for(i = low;i<high;++i)
    {
        for(j = i+1;j<high;++j)
        {
            for(k = j+1;k<high;++k)
            {
                if(((i*i)+(j*j))==(k*k))
                {
                    printf("Triple #%3i: %3i,%3i,%3i --> %4i + %4i = %4i\n",
                    count,i,j,k,(i*i),(j*j),(k*k));
                    ++count;
                }
            }
        }
    }
}