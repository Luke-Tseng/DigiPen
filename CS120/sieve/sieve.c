#include <stdio.h> /* printf      */
#include <math.h>  /* sqrt        */
#include "sieve.h" /* TRUE, FALSE */

void sieve(int array[], int size)
{   
    int k;
    int j;
    int i;
    for(i = 2; i<=size;++i) {
        array[i] = 1;
    }
    for(k = 2;k<size;++k) {
        for(j = k+1; j<=size;++j) {
            if(j%k == 0 ) {
                array[j] = 0;
            }
        }
    }
    
}

int twin_primes(const int primes[], int size)
{
    int count = 0;
    int i;
    int last = 3;
    for(i = 0;i<= size;++i) {
        if(primes[i] == 1){
            if(i-2 == last) {
                ++count;
                printf("twin prime #%4i: %4i and %4i\n",count,last,i);
            }
            last = i;
        }
    }
    return count;
}

double brun_constant(const int primes[], int size)
{
    double brun = 0;
    int i;
    int last = 3;
     for(i = 0;i<= size;++i) {
        if(primes[i] == 1){
            if(i-2 == last) {
                brun += (1/(double)last + 1/(double)i);
            }
            last = i;
        }
    }
  return brun;
}    