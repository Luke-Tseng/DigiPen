#include <stdio.h>
#include "PRNG.h"

int main(void) {
    int num;
    int j;
    int check = 0;
    int i;
    int rolls;
    int count=0;
    

    do{
        printf("How many times do you want to roll the dice? (0=quit) ");
        scanf("%d", &rolls);
        ut_srand(0,0);
        for(i =0;i<rolls;++i) {
            num = RandomInt(1,6);
            check = 0;
            for(j = 1;j<=4;++j) {

                if(num != RandomInt(1,6)) 
                   check++;
            }
            if(check == 0){
                ++count;
            }
        }
        printf("After %d rolls, you had 5 of a kind %d times.\n",rolls,count);
        printf("This is a percentage of %.8f.\n",(float)count/(float)rolls);
    }while(rolls != 0);

    return 0;
}