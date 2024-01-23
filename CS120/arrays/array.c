/******************************************************************************
filename    arrays.c
author      Luke Tseng
DP email    luke.tseng@digipen.edu
course      CS120
section     B
Assignment  2
due date    10/21/2021

Brief Description:
  This program has functions for arrays.
  
******************************************************************************/

void reverse_array(int a[], int size); 
/* Given an array, reverse the order of the elements in
the array. Do not create another array in the function. */
void add_arrays(const int a[], const int b[], int c[], int size);
/* Given three arrays, add the elements of the first two
arrays and put the sum in the third array. */
void scalar_multiply(int a[], int size, int multiplier);
/* Given an array and a multiplier, multiply each
element by the multiplier. */
int dot_product(const int a[], const int b[], int size);
/* Given two arrays, determine the dot product
(multiply each corresponding element and sum the
products). Return the value. */
void cross_product(const int a[], const int b[], int c[]);
/* Given three arrays, determine the cross product of the
first two. The cross product is another array and will
be placed into the third array. The size of all three
arrays will always be 3. */

void copyArray(int a[], int b[], int size) { /* copies an array */
  int i;
  for (i = 0; i < size; ++i) {
    b[i] = a[i];
  }
}

void reverse_array(int a[], int size) {
    int i;
    int j = size-1;
    int temp[1000000];
    copyArray(a, temp, size); /* copies a array to the temp array */
    for(i = 0; i < size;++i) {
        a[i] = temp[j]; /* switches values of the arrays to reverse a*/
        --j;
    }
}

void add_arrays(const int a[], const int b[], int c[], int size) {
    int i;
    for(i = 0; i < size; ++i) {
        c[i] = a[i] + b[i]; /* adds arrays values together */
    }
}

void scalar_multiply(int a[], int size, int multiplier) {
    int i;
    for(i = 0; i < size; ++i) {
        a[i] = a[i] * multiplier; /* multiplies every 
                                     value with the multiplier */
    }
}

int dot_product(const int a[], const int b[], int size) {
    int sum = 0;
    int i;
    for (i = 0; i < size; ++i) {
        sum += a[i] * b[i]; /* multiplies the values between the arrays
                               and adds to the total*/
    }
    return sum;
}
 
void cross_product(const int a[], const int b[], int c[]) {
    c[0] = (a[1] * b[2]) - (a[2] * b[1]); /* first value of cross prduct */
    c[1] = (a[2] * b[0]) - (a[0] * b[2]); /* second value of cross prduct */
    c[2] = (a[0] * b[1]) - (a[1] * b[0]); /* third value of cross prduct */
}