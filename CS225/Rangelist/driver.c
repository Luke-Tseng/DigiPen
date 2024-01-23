#include "rangelist.h"
#include <stdlib.h> /* NULL */
#include <stdio.h> /* printf/sscanf */

#define SANITY_CHECK 1 /* online grading this will be 0 */

/* prototypes */
void print_using_get( struct RangeNode const * p, char const * format, int dimension );
void test0(void);
void test1(void);
void test2(void);
void test3(void);
void test4(void);
void test5(void);
void test6(void);
void test7(void);
void test8(void);
void test9(void);
void test10(void);
void test11(void);
void test12(void);
void test13(void);
void test14(void);
void test15(void);
void test16(void);

/* implementations */

/* very inefficient! Mostly to check the correctness of get's implementation
 * get, which has to find value by traversing the list
 * is called multiple times
 */
void print_using_get( struct RangeNode const * p, char const * format, int dimension )
{
  int i;
  printf( "print_using_get\n" );
  for ( i=0; i<dimension; ++i ) {
    printf( format, i );
  }
  printf( "\n" );
  for ( i=0; i<dimension; ++i ) {
    printf( format, get( p, i ) );
  }
}

/* testing sparse feature
 * all nodes' ranges are a single positions [x,x+1)
 * and never next to each other.
 * So no splitting, merging, range updating is needed.
 *
 * What is needed: basic linked list functionality plus
 * 1) never write zero as a value in the node
 * 2) if non-zero value is overwritten with a zero - delete the node
 */

/* basic linked list functionality */
void test0(void) 
{
  struct RangeNode * prl = NULL;

  printf( "insert 1 at position 10\n" );
  insert_value( &prl, 10, 1 ); /* empty list */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 2\n" );
  insert_value( &prl, 2, 1 ); /* insert front */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 5\n" );
  insert_value( &prl, 5, 1 ); /* insert in between 2 existing nodes */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 12\n" );
  insert_value( &prl, 12, 1 ); /* insert in the end */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 0\n" );
  insert_value( &prl, 0, 1 ); /* insert front */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 7\n" );
  insert_value( &prl, 7, 1 ); /* insert in between 2 existing nodes */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 18\n" );
  insert_value( &prl, 18, 1 ); /* insert in the end */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  delete( prl );
}

/* test get */
void test1() 
{
  struct RangeNode * prl = NULL;
  insert_value( &prl, 10, 1 );  /* empty list */
  insert_value( &prl, 2, 1 );   /* insert front */
  insert_value( &prl, 5, 1 );   /* insert in between 2 existing nodes */
  insert_value( &prl, 12, 1 );  /* insert in the end */
  insert_value( &prl, 0, 1 );   /* insert front */
  insert_value( &prl, 7, 1 );   /* insert in between 2 existing nodes */
  insert_value( &prl, 18, 1 );  /* insert in the end */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  print_using_get( prl, "%3i", 20 );
  printf( "\n" );

  delete( prl );
}

/* overwrite with zero */
void test2() 
{
  struct RangeNode * prl = NULL;
  printf( "hi\n" );
  insert_value( &prl, 10, 1 );  /* empty list */
  insert_value( &prl, 2, 1 );   /* insert front */
  insert_value( &prl, 5, 1 );   /* insert in between 2 existing nodes */
  insert_value( &prl, 12, 1 );  /* insert in the end */
  insert_value( &prl, 0, 1 );   /* insert front */
  insert_value( &prl, 7, 1 );   /* insert in between 2 existing nodes */
  insert_value( &prl, 18, 1 );  /* insert in the end */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "delete position 10\n" );
  insert_value( &prl, 10, 0 );  /* delete in the middle */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );
  
  printf( "delete position 0\n" );
  insert_value( &prl, 0, 0 );  /* delete in front */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );
  
  printf( "delete position 18\n" );
  insert_value( &prl, 18, 0 );  /* delete in the end */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  print_using_get( prl, "%3i", 20 );
  printf( "\n" );

  printf( "delete position 7\n" );
  insert_value( &prl, 7, 0 );
  printf( "delete position 2\n" );
  insert_value( &prl, 2, 0 );
  printf( "delete position 12\n" );
  insert_value( &prl, 12, 0 );
  printf( "delete position 5\n" );
  insert_value( &prl, 5, 0 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  print_using_get( prl, "%3i", 20 );
  printf( "\n" );


  delete( prl );
}

/* write zero to non-existing node (overwrite 0 with 0) */
void test3() 
{
  struct RangeNode * prl = NULL;
  printf( "hi\n" );
  insert_value( &prl, 7, 1 );
  insert_value( &prl, 10, 1 );
  insert_value( &prl, 2, 1 );
  insert_value( &prl, 18, 1 );
  insert_value( &prl, 12, 1 );
  insert_value( &prl, 0, 1 );
  insert_value( &prl, 5, 1 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "write zero to 17\n" );
  insert_value( &prl, 17, 0 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );
  
  printf( "write zero to 1\n" );
  insert_value( &prl, 1, 0 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "write zero to 1000\n" );
  insert_value( &prl, 1000, 0 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  delete( prl );
}

/* overwrite non-zero with non-zero - change value inside a node */
void test4() 
{
  struct RangeNode * prl = NULL;
  printf( "hi\n" );
  insert_value( &prl, 18, 1 );
  insert_value( &prl, 0, 1 );
  insert_value( &prl, 7, 1 );
  insert_value( &prl, 12, 1 );
  insert_value( &prl, 5, 1 );
  insert_value( &prl, 10, 1 );
  insert_value( &prl, 2, 1 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "write 2 to position 0\n" );
  insert_value( &prl, 2, 2 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );
  
  printf( "write 12 to position 12\n" );
  insert_value( &prl, 12, 12 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "write 7 to position 7\n" );
  insert_value( &prl, 7, 7 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "write 18 to position 18\n" );
  insert_value( &prl, 18, 18 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "write -1 to position 0\n" );
  insert_value( &prl, 0, -1 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  delete( prl );
}

/* larger test, combining test0-4 */
void test5() 
{
  struct RangeNode * prl = NULL;
  printf( "hi\n" );
  insert_value( &prl, 10, 1 ); /* empty list */
  insert_value( &prl, 2, 1 ); /* insert front */
  insert_value( &prl, 5, 1 ); /* insert in between 2 existing nodes */
  insert_value( &prl, 12, 1 ); /* insert in the end */
  insert_value( &prl, 0, 1 ); /* insert front */
  insert_value( &prl, 7, 1 ); /* insert in between 2 existing nodes */
  insert_value( &prl, 18, 1 ); /* insert in the end */
  print_using_get( prl, "%3i", 30 );
  printf( "\n" );
  delete( prl );
  prl = NULL;

  insert_value( &prl, 10, 1 );  /* empty list */
  insert_value( &prl, 2, 1 );   /* insert front */
  insert_value( &prl, 5, 1 );   /* insert in between 2 existing nodes */
  insert_value( &prl, 12, 1 );  /* insert in the end */
  insert_value( &prl, 0, 1 );   /* insert front */
  insert_value( &prl, 7, 1 );   /* insert in between 2 existing nodes */
  insert_value( &prl, 18, 1 );  /* insert in the end */
  print_using_get( prl, "%3i", 30 );
  printf( "\n" );
  delete( prl );
  prl = NULL;

  insert_value( &prl, 10, 1 );  /* empty list */
  insert_value( &prl, 2, 1 );   /* insert front */
  insert_value( &prl, 5, 1 );   /* insert in between 2 existing nodes */
  insert_value( &prl, 12, 1 );  /* insert in the end */
  insert_value( &prl, 0, 1 );   /* insert front */
  insert_value( &prl, 7, 1 );   /* insert in between 2 existing nodes */
  insert_value( &prl, 18, 1 );  /* insert in the end */
  insert_value( &prl, 10, 0 );  /* delete in the middle */
  insert_value( &prl, 0, 0 );  /* delete in front */
  insert_value( &prl, 18, 0 );  /* delete in the end */
  print_using_get( prl, "%3i", 30 );
  printf( "\n" );

  insert_value( &prl, 7, 0 );
  insert_value( &prl, 2, 0 );
  insert_value( &prl, 12, 0 );
  insert_value( &prl, 5, 0 );
  print_using_get( prl, "%3i", 30 );
  printf( "\n" );

  delete( prl );
  prl = NULL;

  insert_value( &prl, 7, 1 );
  insert_value( &prl, 10, 1 );
  insert_value( &prl, 2, 1 );
  insert_value( &prl, 18, 1 );
  insert_value( &prl, 12, 1 );
  insert_value( &prl, 0, 1 );
  insert_value( &prl, 5, 1 );
  printf_elements( prl, "%3i", 30, SANITY_CHECK );
  printf( "\n" );

  insert_value( &prl, 17, 0 );
  insert_value( &prl, 1, 0 );
  insert_value( &prl, 1000, 0 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 30, SANITY_CHECK );
  printf( "\n" );

  delete( prl );
  prl = NULL;

  insert_value( &prl, 18, 1 );
  insert_value( &prl, 0, 1 );
  insert_value( &prl, 7, 1 );
  insert_value( &prl, 12, 1 );
  insert_value( &prl, 5, 1 );
  insert_value( &prl, 10, 1 );
  insert_value( &prl, 2, 1 );
  insert_value( &prl, 2, 2 );
  insert_value( &prl, 12, 12 );
  insert_value( &prl, 7, 7 );
  insert_value( &prl, 18, 18 );
  insert_value( &prl, 0, -1 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 30, 1 );
  printf( "\n" );

  delete( prl );
}

/* stress test for inserts and deletes.
 * Given indices 0 2 4 6 8 .... , create a list of values at those indices
 * by inserting values in all possible orders (permutations) 
 * and for each of those delete in all possible orders. 
 * There are n! possible permutations of n positions,
 * so runtime is n! * n! lists created and destroyed:
 * n = 5 => 120*120=14400
 */
void test6() 
{

  int permutations[120][5] = {
    {1,3,5,7,9}, {1,3,5,9,7}, {1,3,7,5,9}, {1,3,7,9,5}, {1,3,9,5,7}, {1,3,9,7,5}, {1,5,3,7,9}, {1,5,3,9,7}, {1,5,7,3,9},
    {1,5,7,9,3}, {1,5,9,3,7}, {1,5,9,7,3}, {1,7,3,5,9}, {1,7,3,9,5}, {1,7,5,3,9}, {1,7,5,9,3}, {1,7,9,3,5}, {1,7,9,5,3},
    {1,9,3,5,7}, {1,9,3,7,5}, {1,9,5,3,7}, {1,9,5,7,3}, {1,9,7,3,5}, {1,9,7,5,3}, {3,1,5,7,9}, {3,1,5,9,7}, {3,1,7,5,9},
    {3,1,7,9,5}, {3,1,9,5,7}, {3,1,9,7,5}, {3,5,1,7,9}, {3,5,1,9,7}, {3,5,7,1,9}, {3,5,7,9,1}, {3,5,9,1,7}, {3,5,9,7,1},
    {3,7,1,5,9}, {3,7,1,9,5}, {3,7,5,1,9}, {3,7,5,9,1}, {3,7,9,1,5}, {3,7,9,5,1}, {3,9,1,5,7}, {3,9,1,7,5}, {3,9,5,1,7},
    {3,9,5,7,1}, {3,9,7,1,5}, {3,9,7,5,1}, {5,1,3,7,9}, {5,1,3,9,7}, {5,1,7,3,9}, {5,1,7,9,3}, {5,1,9,3,7}, {5,1,9,7,3},
    {5,3,1,7,9}, {5,3,1,9,7}, {5,3,7,1,9}, {5,3,7,9,1}, {5,3,9,1,7}, {5,3,9,7,1}, {5,7,1,3,9}, {5,7,1,9,3}, {5,7,3,1,9},
    {5,7,3,9,1}, {5,7,9,1,3}, {5,7,9,3,1}, {5,9,1,3,7}, {5,9,1,7,3}, {5,9,3,1,7}, {5,9,3,7,1}, {5,9,7,1,3}, {5,9,7,3,1},
    {7,1,3,5,9}, {7,1,3,9,5}, {7,1,5,3,9}, {7,1,5,9,3}, {7,1,9,3,5}, {7,1,9,5,3}, {7,3,1,5,9}, {7,3,1,9,5}, {7,3,5,1,9},
    {7,3,5,9,1}, {7,3,9,1,5}, {7,3,9,5,1}, {7,5,1,3,9}, {7,5,1,9,3}, {7,5,3,1,9}, {7,5,3,9,1}, {7,5,9,1,3}, {7,5,9,3,1},
    {7,9,1,3,5}, {7,9,1,5,3}, {7,9,3,1,5}, {7,9,3,5,1}, {7,9,5,1,3}, {7,9,5,3,1}, {9,1,3,5,7}, {9,1,3,7,5}, {9,1,5,3,7},
    {9,1,5,7,3}, {9,1,7,3,5}, {9,1,7,5,3}, {9,3,1,5,7}, {9,3,1,7,5}, {9,3,5,1,7}, {9,3,5,7,1}, {9,3,7,1,5}, {9,3,7,5,1},
    {9,5,1,3,7}, {9,5,1,7,3}, {9,5,3,1,7}, {9,5,3,7,1}, {9,5,7,1,3}, {9,5,7,3,1}, {9,7,1,3,5}, {9,7,1,5,3}, {9,7,3,1,5},
    {9,7,3,5,1}, {9,7,5,1,3}, {9,7,5,3,1}
  };

  int p1,p2,i;
  struct RangeNode * prl = NULL;
    printf( "hi\n" );
  for ( p1 = 0; p1 < 120; ++p1 ) {
    for ( p2 = 0; p2 < 120; ++p2 ) {
      /* insert 5 elements in order p1 */
      for ( i = 0; i < 5; ++i ) {
        insert_value( &prl, permutations[ p1 ][ i ] , permutations[ p1 ][ i ] ); /* insert */
      }
      print_raw( prl ); /* should be  1 3 5 7 9 (at positions 1 3 5 7 9 */
      printf( "\n" );
      /* delete 5 elements in order p2 */
      for ( i = 0; i < 5; ++i ) {
        insert_value( &prl, permutations[ p2 ][ i ] , 0 ); /* delete */
      }
      print_raw( prl ); /* should be empty */
      printf( "\n" );
    }
  }
}

/* range related tests */
void test7(void) 
{
  struct RangeNode * prl = NULL;
  insert_nonoverlapping_range( &prl, 13, 16, 7 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 10\n" );
  insert_value( &prl, 10, 1 ); /* in front of a group */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  delete( prl );
}

void test8(void) 
{
  struct RangeNode * prl = NULL;
  insert_nonoverlapping_range( &prl, 3, 6, 7 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 10\n" );
  insert_value( &prl, 10, 1 ); /* in the end of the list */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  delete( prl );
}

void test9(void) 
{
  struct RangeNode * prl = NULL;
  insert_nonoverlapping_range( &prl, 3, 6, 7 );
  insert_nonoverlapping_range( &prl, 13, 16, 17 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 10\n" );
  insert_value( &prl, 10, 1 ); /* in the middle of the list */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  delete( prl );
}

/* updating existing nodes */
void test10(void) 
{
  struct RangeNode * prl = NULL;
  insert_nonoverlapping_range( &prl, 3, 6, 8 );
  insert_nonoverlapping_range( &prl, 13, 16, 9 );
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 8 at position 6\n" );
  insert_value( &prl, 6, 8 ); /* just update range [3,6) to [3,7) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 9 at position 12\n" );
  insert_value( &prl, 12, 9 ); /* just update range [13,16) to [12,16) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 8 at position 2\n" );
  insert_value( &prl, 2, 8 ); /* just update range [3,7) to [2,7) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 9 at position 16\n" );
  insert_value( &prl, 16, 9 ); /* just update range [12,16) to [12,17) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  delete( prl );
}

/* updating nodes + merging */
void test11(void) 
{
  struct RangeNode * prl = NULL;
  insert_nonoverlapping_range( &prl, 3, 6, 4 );
  insert_nonoverlapping_range( &prl, 8, 12, 4 ); /* same value */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 4 at position 7\n" );
  insert_value( &prl, 7, 4 ); /* just update range [8,12) to [7,12) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 4 at position 6\n" );
  insert_value( &prl, 6, 4 ); /* update range [7,12) to [6,12), merge [3, 6)+[6,12) = [3,12)*/
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  delete( prl );
}

/* splitting nodes - 1 */
void test12(void) 
{
  struct RangeNode * prl = NULL;
  insert_nonoverlapping_range( &prl, 3, 8, 4 );
  insert_nonoverlapping_range( &prl, 10, 14, 6 ); 
  insert_nonoverlapping_range( &prl, 17, 20, 8 ); 
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 5 at position 6\n" );
  insert_value( &prl, 6, 5 ); /* split [3, 8) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 5 at position 12\n" );
  insert_value( &prl, 12, 5 ); /* split [10, 14) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 5 at position 18\n" );
  insert_value( &prl, 18, 5 ); /* split [17, 20) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  delete( prl );
}

/* splitting nodes - 2 (new value's position = begin ) */
void test13(void) 
{
  struct RangeNode * prl = NULL;
  insert_nonoverlapping_range( &prl, 3, 8, 4 );
  insert_nonoverlapping_range( &prl, 10, 14, 6 ); 
  insert_nonoverlapping_range( &prl, 17, 20, 8 ); 
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 5 at position 3\n" );
  insert_value( &prl, 3, 5 ); /* split [3, 8) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 5 at position 10\n" );
  insert_value( &prl, 10, 5 ); /* split [10, 14) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 5 at position 17\n" );
  insert_value( &prl, 17, 5 ); /* split [17, 20) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  delete( prl );
}

/* splitting nodes - 3 (new value's position = end -1 ) */
void test14(void) 
{
  struct RangeNode * prl = NULL;
  insert_nonoverlapping_range( &prl, 3, 8, 4 );
  insert_nonoverlapping_range( &prl, 10, 14, 6 ); 
  insert_nonoverlapping_range( &prl, 17, 20, 8 ); 
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 5 at position 7\n" );
  insert_value( &prl, 7, 5 ); /* split [3, 8) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 5 at position 13\n" );
  insert_value( &prl, 13, 5 ); /* split [10, 14) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 5 at position 19\n" );
  insert_value( &prl, 19, 5 ); /* split [17, 20) */
  print_raw( prl );
  printf( "\n" );
  printf_elements( prl, "%3i", 20, 1 );
  printf( "\n" );

  delete( prl );
}

/*
 * Stress test for ranges:
 * similar to test5, but all positions are next to each other, so merging and splitting 
 * in all possible ways will be checked, but will NOT happen (all values are different)
 */
void test15() 
{
  int permutations[120][5] = {
    {1,2,3,4,5}, {1,2,3,5,4}, {1,2,4,3,5}, {1,2,4,5,3}, {1,2,5,3,4}, {1,2,5,4,3}, {1,3,2,4,5}, {1,3,2,5,4}, {1,3,4,2,5},
    {1,3,4,5,2}, {1,3,5,2,4}, {1,3,5,4,2}, {1,4,2,3,5}, {1,4,2,5,3}, {1,4,3,2,5}, {1,4,3,5,2}, {1,4,5,2,3}, {1,4,5,3,2},
    {1,5,2,3,4}, {1,5,2,4,3}, {1,5,3,2,4}, {1,5,3,4,2}, {1,5,4,2,3}, {1,5,4,3,2}, {2,1,3,4,5}, {2,1,3,5,4}, {2,1,4,3,5},
    {2,1,4,5,3}, {2,1,5,3,4}, {2,1,5,4,3}, {2,3,1,4,5}, {2,3,1,5,4}, {2,3,4,1,5}, {2,3,4,5,1}, {2,3,5,1,4}, {2,3,5,4,1},
    {2,4,1,3,5}, {2,4,1,5,3}, {2,4,3,1,5}, {2,4,3,5,1}, {2,4,5,1,3}, {2,4,5,3,1}, {2,5,1,3,4}, {2,5,1,4,3}, {2,5,3,1,4},
    {2,5,3,4,1}, {2,5,4,1,3}, {2,5,4,3,1}, {3,1,2,4,5}, {3,1,2,5,4}, {3,1,4,2,5}, {3,1,4,5,2}, {3,1,5,2,4}, {3,1,5,4,2},
    {3,2,1,4,5}, {3,2,1,5,4}, {3,2,4,1,5}, {3,2,4,5,1}, {3,2,5,1,4}, {3,2,5,4,1}, {3,4,1,2,5}, {3,4,1,5,2}, {3,4,2,1,5},
    {3,4,2,5,1}, {3,4,5,1,2}, {3,4,5,2,1}, {3,5,1,2,4}, {3,5,1,4,2}, {3,5,2,1,4}, {3,5,2,4,1}, {3,5,4,1,2}, {3,5,4,2,1},
    {4,1,2,3,5}, {4,1,2,5,3}, {4,1,3,2,5}, {4,1,3,5,2}, {4,1,5,2,3}, {4,1,5,3,2}, {4,2,1,3,5}, {4,2,1,5,3}, {4,2,3,1,5},
    {4,2,3,5,1}, {4,2,5,1,3}, {4,2,5,3,1}, {4,3,1,2,5}, {4,3,1,5,2}, {4,3,2,1,5}, {4,3,2,5,1}, {4,3,5,1,2}, {4,3,5,2,1},
    {4,5,1,2,3}, {4,5,1,3,2}, {4,5,2,1,3}, {4,5,2,3,1}, {4,5,3,1,2}, {4,5,3,2,1}, {5,1,2,3,4}, {5,1,2,4,3}, {5,1,3,2,4},
    {5,1,3,4,2}, {5,1,4,2,3}, {5,1,4,3,2}, {5,2,1,3,4}, {5,2,1,4,3}, {5,2,3,1,4}, {5,2,3,4,1}, {5,2,4,1,3}, {5,2,4,3,1},
    {5,3,1,2,4}, {5,3,1,4,2}, {5,3,2,1,4}, {5,3,2,4,1}, {5,3,4,1,2}, {5,3,4,2,1}, {5,4,1,2,3}, {5,4,1,3,2}, {5,4,2,1,3},
    {5,4,2,3,1}, {5,4,3,1,2}, {5,4,3,2,1}
  };

  int p1,p2,i;
  struct RangeNode * prl = NULL;

  for ( p1 = 0; p1 < 120; ++p1 ) {
    for ( p2 = 0; p2 < 120; ++p2 ) {
      /* insert 5 elements in order p1 */
      for ( i = 0; i < 5; ++i ) {
        insert_value( &prl, permutations[ p1 ][ i ] , permutations[ p1 ][ i ] ); /* insert */
      }
      print_raw( prl ); /* should be  1 2 3 4 5 (at positions 1 2 3 4 5 */
      printf( "\n" );
      /* delete 5 elements in order p2 */
      for ( i = 0; i < 5; ++i ) {
#if 0
        printf( "delete %i\n", permutations[ p2 ][ i ] );
#endif 
        insert_value( &prl, permutations[ p2 ][ i ] , 0 ); /* delete */
#if 0
      print_raw( prl ); /* should be  1 3 5 7 9 (at positions 1 3 5 7 9 */
      printf( "\n" );
#endif 
      }
      print_raw( prl ); /* should be empty */
      printf( "\n" );
    }
  }
}

/*
 * Stress test for ranges:
 * similar to test5, but all positions are next to each other, so merging and splitting 
 * in all possible ways will be checked, and will happen, since the values at all 
 * positions are same - 8
 */
void test16() 
{
  int permutations[120][5] = {
    {1,2,3,4,5}, {1,2,3,5,4}, {1,2,4,3,5}, {1,2,4,5,3}, {1,2,5,3,4}, {1,2,5,4,3}, {1,3,2,4,5}, {1,3,2,5,4}, {1,3,4,2,5},
    {1,3,4,5,2}, {1,3,5,2,4}, {1,3,5,4,2}, {1,4,2,3,5}, {1,4,2,5,3}, {1,4,3,2,5}, {1,4,3,5,2}, {1,4,5,2,3}, {1,4,5,3,2},
    {1,5,2,3,4}, {1,5,2,4,3}, {1,5,3,2,4}, {1,5,3,4,2}, {1,5,4,2,3}, {1,5,4,3,2}, {2,1,3,4,5}, {2,1,3,5,4}, {2,1,4,3,5},
    {2,1,4,5,3}, {2,1,5,3,4}, {2,1,5,4,3}, {2,3,1,4,5}, {2,3,1,5,4}, {2,3,4,1,5}, {2,3,4,5,1}, {2,3,5,1,4}, {2,3,5,4,1},
    {2,4,1,3,5}, {2,4,1,5,3}, {2,4,3,1,5}, {2,4,3,5,1}, {2,4,5,1,3}, {2,4,5,3,1}, {2,5,1,3,4}, {2,5,1,4,3}, {2,5,3,1,4},
    {2,5,3,4,1}, {2,5,4,1,3}, {2,5,4,3,1}, {3,1,2,4,5}, {3,1,2,5,4}, {3,1,4,2,5}, {3,1,4,5,2}, {3,1,5,2,4}, {3,1,5,4,2},
    {3,2,1,4,5}, {3,2,1,5,4}, {3,2,4,1,5}, {3,2,4,5,1}, {3,2,5,1,4}, {3,2,5,4,1}, {3,4,1,2,5}, {3,4,1,5,2}, {3,4,2,1,5},
    {3,4,2,5,1}, {3,4,5,1,2}, {3,4,5,2,1}, {3,5,1,2,4}, {3,5,1,4,2}, {3,5,2,1,4}, {3,5,2,4,1}, {3,5,4,1,2}, {3,5,4,2,1},
    {4,1,2,3,5}, {4,1,2,5,3}, {4,1,3,2,5}, {4,1,3,5,2}, {4,1,5,2,3}, {4,1,5,3,2}, {4,2,1,3,5}, {4,2,1,5,3}, {4,2,3,1,5},
    {4,2,3,5,1}, {4,2,5,1,3}, {4,2,5,3,1}, {4,3,1,2,5}, {4,3,1,5,2}, {4,3,2,1,5}, {4,3,2,5,1}, {4,3,5,1,2}, {4,3,5,2,1},
    {4,5,1,2,3}, {4,5,1,3,2}, {4,5,2,1,3}, {4,5,2,3,1}, {4,5,3,1,2}, {4,5,3,2,1}, {5,1,2,3,4}, {5,1,2,4,3}, {5,1,3,2,4},
    {5,1,3,4,2}, {5,1,4,2,3}, {5,1,4,3,2}, {5,2,1,3,4}, {5,2,1,4,3}, {5,2,3,1,4}, {5,2,3,4,1}, {5,2,4,1,3}, {5,2,4,3,1},
    {5,3,1,2,4}, {5,3,1,4,2}, {5,3,2,1,4}, {5,3,2,4,1}, {5,3,4,1,2}, {5,3,4,2,1}, {5,4,1,2,3}, {5,4,1,3,2}, {5,4,2,1,3},
    {5,4,2,3,1}, {5,4,3,1,2}, {5,4,3,2,1}
  };

  int p1,p2,i;
  struct RangeNode * prl = NULL;

  for ( p1 = 0; p1 < 120; ++p1 ) {
    for ( p2 = 0; p2 < 120; ++p2 ) {
      /* insert 5 elements in order p1 */
      for ( i = 0; i < 5; ++i ) {
        insert_value( &prl, permutations[ p1 ][ i ] , 8 ); /* insert */
      }
      print_raw( prl ); /* should be  1 2 3 4 5 (at positions 1 2 3 4 5 */
      printf( "\n" );
      /* delete 5 elements in order p2 */
      for ( i = 0; i < 5; ++i ) {
#if 0
        printf( "delete %i\n", permutations[ p2 ][ i ] );
#endif 
        insert_value( &prl, permutations[ p2 ][ i ] , 0 ); /* delete */
#if 0
      print_raw( prl ); /* should be  1 3 5 7 9 (at positions 1 3 5 7 9 */
      printf( "\n" );
#endif 
      }
      print_raw( prl ); /* should be empty */
      printf( "\n" );
    }
  }
}

void (*pTests[])(void) = {
  test0, test1, test2, test3, test4, test5, test6, test7, test8, test9,
  test10, test11, test12, test13, test14, test15, test16
}; 

int main (int argc, char ** argv) 
{
  if (argc >1) {
    int test = 0;
    sscanf(argv[1],"%i",&test);
    pTests[test]();
  }
  return 0;
}
