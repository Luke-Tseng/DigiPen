/******************************************************************************
filename    dates.c
author      Luke Tseng
DP email    luke.tseng@digipen.edu
course      CS120
section     B
lab week    4
due date    10/09/2021

Brief Description:
  This program returns which of the 2 inputted dates comes first.
******************************************************************************/
#include <stdio.h>

int main(void) {
    int month1;
    int day1;
    int year1;
    int month2;
    int day2;
    int year2;
    int result;
    printf("Enter the first date: (mm/dd/yyyy) ");
    scanf("%d/%d/%d",&month1,&day1,&year1);
    printf("Enter the second date: (mm/dd/yyyy) ");
    scanf("%d/%d/%d",&month2,&day2,&year2);
    if(year1 > year2) {
        result = 2;
    } else if (year2 > year1) {
        result = 1;
    } else if (month1 > month2){
        result = 2;
    } else if (month2 > month1) {
        result = 1;
    } else if (day1 > day2) {
        result = 2;
    } else if (day2 > day1){
        result = 1;
    } else {
        result = 3;
    }
    
    switch(result) {
        case 1: 
        printf("%02d/%02d/%d comes before %02d/%02d/%d.\n",
        month1,day1,year1,month2,day2,year2);
        break;
    case 2: 
        printf("%02d/%02d/%d comes after %02d/%02d/%d.\n",
        month1,day1,year1,month2,day2,year2);
        break;
    case 3: 
        printf("%02d/%02d/%d and %02d/%02d/%d are the same date.\n",
        month1,day1,year1,month2,day2,year2);
        break;
    }
    return 0;
}
