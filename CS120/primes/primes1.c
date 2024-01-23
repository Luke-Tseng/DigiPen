#include <stdio.h>

#define FALSE 0
#define TRUE  1

int is_prime(int number) {
    int i;
    if(number == 1) {
        return FALSE;
    }
    if(number == 2) {
        return TRUE;
    } else if(number % 2 == 0) {
        return FALSE;
    }
    for(i = 3;i<=number/2;i+=2) {
        if(number%i == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

void conjecture(int low, int high) {
    int i, j ,k;
    for(i = low;i<= high;i+=2) {
        for(j=i-1;j>=2;--j){
            if(is_prime(j)) { 
                for(k = 2;k<= j;++k) {
                    if(is_prime(k)){
                        if(j+k == i) {
                            printf("%3d = %3d + %3d\n",i,k,j);
                            j=2;
                        }
                    }
                }
            }
        }
    }
}