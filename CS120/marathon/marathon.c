/******************************************************************************
filename    marathon.c
author      Luke Tseng
DP email    luke.tseng@digipen.edu
course      CS120
section     B
lab week    2
due date    9/25/2021

Brief Description:
  This program calculates the distance of a marathon.
  
******************************************************************************/

#include <stdio.h> /* printf */

/* Convenient definitions */
#define YARDS_PER_MILE 1760
#define KILOS_PER_MILE 1.609

/* A marathon is 26 miles, 385 yards               */
/* Prints the distance of a marathon in kilometers */
int main(void)
{
  int miles = 26;    /* Miles in a marathon                 */
  int yards = 385;   /* Yards in a marathon                 */
  double kilometers; /* Calculated kilometers in a marathon */
  
    /* Convert miles and yards into kilometers */
  kilometers = (miles + (double)yards / YARDS_PER_MILE) * KILOS_PER_MILE;

    /* Display the result on the screen */
  printf("A marathon is %f kilometers\n", kilometers);
  
    /* Return successful value to OS */
  return 0;
}
