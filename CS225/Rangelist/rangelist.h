#ifndef RANGELIST_H
#define RANGELIST_H

/* each node represents a range of equal values from begin to end,
 * similar to STL begin is included and end is NOT
 * Example RangeNode{2,6,3} is
 * 0 1 2 3 4 5 6 7 8 9 index
 * 0 0 3 3 3 3 0 0 0 0 value 
 */
struct RangeNode {
  int begin;
  int end;
  int value;
  struct RangeNode * next;
};

/* insert a single value */
int insert_value( struct RangeNode ** ppHead, int position, int value );

/* return value at position (index) */
int get( struct RangeNode const * p, int position );

/* deallocate */
void delete( struct RangeNode * p);

/* FUNCTION BELOW ARE IMPLEMENTED - DO NOT CHANGE */

/* insert a range, assume there are no overlaps with the existing ranges
 * this is a dangerous function, it better be a hidden helper function 
 * (private in C++ or declared in the header file in C)
 * I will use this function to test your code in the case you did 
 * not implement / or incompletely the insert_value above
 *
 * Return value: error code (1 = allocation error, 0 - OK 
 */
int insert_nonoverlapping_range( struct RangeNode ** ppHead, int begin, int end, int value );

/* function below are already implemented */

/* print elements as a vector, print 0's for non-existing positions
 * assume the length of the vector = dim (third argument)
 * doesn't check if the list contains positions beyond dim 
 * if do_sanity_check is non-zero thie function will do some 
 * error checking (notie - this is not a complete analysis) 
 * not ll errors will be detected */
void printf_elements( struct RangeNode const * p, char * format, int dimension, int do_sanity_check ); 

/* raw dump of the list data */
void print_raw( struct RangeNode const * const); /* no zeros */

#endif
