#include <stdio.h>     /* printf      */
#include <ctype.h>     /* toupper     */
#include <string.h>    /* strlen      */
#include "histogram.h" /* lab defines */

int GetCounts(const char *sentence, int letters[], int *spaces, int *other)
{
    
    int count = 0;
    int i;
    int length = strlen(sentence);
    int k;
    *spaces=0;
    *other = 0;
    
    for(k = 0; k<26;++k){
        letters[k] = 0;
    }
    for(i = 0;i<length;++i) {
        if(isalpha(*(sentence+i))) {
            ++letters[toupper(*(sentence+i))-65];
            ++count;
        } else if (*(sentence+i) == ' ') {
            (*spaces)++;
        } else {
            ++(*other);
        }
    }
    return count;
}


void PrintHistogram(const int letters[])
{
    int i;
    int j;
    for(i = 0;i<26;++i) {
        printf("%c:2",(char)(i+65));
        for(j=0;j<letters[i];++j) {
            printf("*");
        }
        printf("\n");
    }
}

void GetStatistics(const int letters[], double *average, char *max_letter, 
                   int *max_count)
{
    int i;
    int count = 0;
    *max_letter = ' ';
    *max_count = 0;

    for(i = 0;i< 26;++i) {
        if(letters[i]>*max_count) {
            *max_letter = (char)(i+65);
            *max_count = letters[i];
        }
        count += letters[i] * (i+1);
    }
    *average = (double)count/26;
}
