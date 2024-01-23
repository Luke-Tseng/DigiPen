/******************************************************************************
filename    numbers.c
author      Luke Tseng
DP email    luke.tseng@digipen.edu
course      CS120
section     B
lab week    3
due date    10/02/2021

Brief Description:
  This program calculates the sin, cos, tan and atan of the 3 inputted numbers.
******************************************************************************/
#include <stdio.h>
#include <math.h>

int main() {
    float f, s, t;
    printf("Enter 3 numbers between 0 and 9.999: \n");
    scanf("%f %f %f", &f, &s, &t);
    printf("Number      sin      cos      tan      atan\n");
    printf("-------------------------------------------\n");
    printf("%.5f%9.3f%9.3f%9.3f%9.3f\n", f, sin(f), cos(f), tan(f), atan(f));
    printf("%.5f%9.3f%9.3f%9.3f%9.3f\n", s, sin(s), cos(s), tan(s), atan(s));
    printf("%.5f%9.3f%9.3f%9.3f%9.3f\n", t, sin(t), cos(t), tan(t), atan(t));
    return 0;
}   