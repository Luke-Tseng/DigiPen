#include <stddef.h> /* NULL */
#include <stdio.h>

/*swap function for the exchange bytes function*/
static void swap(char *left, char *right) {
  char temp = *right;
  *right = *left;
  *left = temp;
}
/*Given two pointers(from and to), scan the range looking for the 
first occurrence of byte, returning a pointer to it.
If byte is not found, return NULL. The pointers are guaranteed to be 
pointing into the same array and that the address of from 
is less than or equal to the address of to.*/
const char *find_byte(const char *from, const char *to, char byte)
{
  int i = 0;
 do {

    if(*(from+i) == byte) {
      return (from+i);
    }
    if((from+i) == to) {    
      break;
    }
    ++i;
    }  while((from+i) != to);

  return NULL;
}

/*Given two pointers (from and to), scan the range looking for the first
occurrence of any byte that is in the bytes array, returning a pointer to it.
If none of the characters in the bytes array is found, return NULL.
The count parameter tells you how many characters are in the bytes array.
The pointers are guaranteed to be pointing into the same array and that 
the address of from is less than or equal to the address of to.*/
const char *find_any_byte(const char *from, const char *to, const char *bytes,
                          int count)
{
  int i = 0;
  int j;
  while(*(from+i) != *to) {
    for(j=0; j<count;++j) {
      if(*(from+i) == *(bytes+j)) {
        return (from+i);
      }
    }
    ++i;
  }
  return NULL;
}

/*Given two pointers(from and to), scan the range counting the number of 
occurrences of byte. The return value is the number of times that byte 
appeared in the range. returning a pointer to it.
The pointers are guaranteed to be pointing into the same array and that
the address of from is less than or equal to the address of to.*/
int count_bytes(const char *from, const char *to, char byte)
{
  int count = 0;
  int i = 0;
  while(*(from+i) != *to) {
    if(*(from+i) == byte) {
      ++count;
    }
    ++i;
  }
  return count;
}
/*Given two pointers, scan the range counting occurrences of any 
character that is in the bytes array. The return value is the sum of all of
the characters found. The count parameter tells you how many characters
are in the bytes array.The pointers are guaranteed to be pointing into
the same array and that the address of from is less than or equal to the
address of to.*/
int count_any_bytes(const char *from, const char *to, const char *bytes,
                    int count)
{
  int counter = 0;
  int i= 0;
  int j;
  while(*(from+i) != *to) {
    for(j=0; j<count;++j) {
      if(*(from+i) == *(bytes+j)) {
        ++counter;
      }
    }
    ++i;
  }
  return counter;
}
/*Given two pointers(location1 and location2), compare each character
byte-by-byteto determine if the ranges are the same. If the bytes are
all the same, the function returns 0. If the the bytes pointed to by
location1 are less than the bytes pointed to by location2, the function
returns a negative number. Otherwise, the function returns a positive
number. The count tells the function how many bytes to compare.
If you don't have a better reason, you should return -1 as the
negative return and 1 as the positive return.*/
int compare_bytes(const char *location1, const char *location2, int count)
{
  int i;
  for(i = 0; i<count;++i) {
    if(*(location1+i)>*(location2+i)) {
      return 1;
    }
    if(*(location1+i)<*(location2+i)){
      return -1;
    }
  }
  return 0;
}

/*Given two pointers(location1 and location2), exchange the bytes from each.
The count parameter tells you how many bytes to swap. You are guaranteed
that the two locations do not overlap in memory. Notice that the pointers
are not marked const. This is because you are modifying the characters in 
both ranges. You are not allowed to create any arrays. You must do the
exchange within the ranges provided.*/
void exchange_bytes(char *p1, char *p2, int count)
{
  int i;
  for(i = 0;i<count;++i) {
    swap((p1+i),(p2+i));
  }  
}

/*Given two pointers(from and to), copy count bytes (characters) from the
from pointer to the to pointer. The count parameter tells you how many bytes
to copy. It is possible that the two ranges overlap, so you must handle this!
This means you will need to think before you write any code. Notice that the
pointers are not marked const, meaning that you are modifying the arrays
in-place. You are not allowed to create any new arrays in your code.*/
void copy_bytes(char *from, char *to, int count)
{
  int i;
  for(i = count-1;i>=0;--i) {
    *(to+i) = *(from+i);
  } 
}