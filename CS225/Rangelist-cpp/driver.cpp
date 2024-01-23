#include "rangelist.h"
#include <cstdlib>    /* malloc/free */
#include <cstddef>    /* NULL */
#include <cstdio>     /* printf/sscanf */
#include <iostream>   /* cin/cout */

#define SANITY_CHECK 0 /* online grading this will be 0 */

/* very inefficient! Mostly to check the correctness of get's implementation
 * get, which has to find value by traversing the list
 * is called multiple times
 */
void print_using_get( RangeList& rl, char const * format, int dimension )
{
  int i;
  printf( "print_using_get\n" );
  for ( i=0; i<dimension; ++i ) {
    printf( format, i );
  }
  printf( "\n" );
  for ( i=0; i<dimension; ++i ) {
    printf( format, rl.get( i ) );
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
  RangeList rl;

  printf( "insert 1 at position 10\n" );
  rl.insert_value(  10, 1 ); /* empty list */
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 2\n" );
  rl.insert_value(  2, 1 ); /* insert front */
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 5\n" );
  rl.insert_value(  5, 1 ); /* insert in between 2 existing nodes */
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 12\n" );
  rl.insert_value(  12, 1 ); /* insert in the end */
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 0\n" );
  rl.insert_value(  0, 1 ); /* insert front */
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 7\n" );
  rl.insert_value(  7, 1 ); /* insert in between 2 existing nodes */
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "insert 1 at position 18\n" );
  rl.insert_value(  18, 1 ); /* insert in the end */
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

}

/* test get */
void test1() 
{
  RangeList rl;

  rl.insert_value(  10, 1 );  /* empty list */
  rl.insert_value(  2, 1 );   /* insert front */
  rl.insert_value(  5, 1 );   /* insert in between 2 existing nodes */
  rl.insert_value(  12, 1 );  /* insert in the end */
  rl.insert_value(  0, 1 );   /* insert front */
  rl.insert_value(  7, 1 );   /* insert in between 2 existing nodes */
  rl.insert_value(  18, 1 );  /* insert in the end */
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  print_using_get( rl, "%3i", 20 );
  printf( "\n" );

}

/* overwrite with zero */
void test2() 
{
  RangeList rl;

  rl.insert_value(  10, 1 );  /* empty list */
  rl.insert_value(  2, 1 );   /* insert front */
  rl.insert_value(  5, 1 );   /* insert in between 2 existing nodes */
  rl.insert_value(  12, 1 );  /* insert in the end */
  rl.insert_value(  0, 1 );   /* insert front */
  rl.insert_value(  7, 1 );   /* insert in between 2 existing nodes */
  rl.insert_value(  18, 1 );  /* insert in the end */
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "delete position 10\n" );
  rl.insert_value(  10, 0 );  /* delete in the middle */
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );
  
  printf( "delete position 0\n" );
  rl.insert_value(  0, 0 );  /* delete in front */
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );
  
  printf( "delete position 18\n" );
  rl.insert_value(  18, 0 );  /* delete in the end */
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  print_using_get( rl, "%3i", 20 );
  printf( "\n" );

  printf( "delete position 7\n" );
  rl.insert_value(  7, 0 );
  printf( "delete position 2\n" );
  rl.insert_value(  2, 0 );
  printf( "delete position 12\n" );
  rl.insert_value(  12, 0 );
  printf( "delete position 5\n" );
  rl.insert_value(  5, 0 );
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  print_using_get( rl, "%3i", 20 );
  printf( "\n" );


}

/* write zero to non-existing node (overwrite 0 with 0) */
void test3() 
{
  RangeList rl;

  rl.insert_value(  7, 1 );
  rl.insert_value(  10, 1 );
  rl.insert_value(  2, 1 );
  rl.insert_value(  18, 1 );
  rl.insert_value(  12, 1 );
  rl.insert_value(  0, 1 );
  rl.insert_value(  5, 1 );
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "write zero to 17\n" );
  rl.insert_value(  17, 0 );
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );
  
  printf( "write zero to 1\n" );
  rl.insert_value(  1, 0 );
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "write zero to 1000\n" );
  rl.insert_value(  1000, 0 );
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

}

/* overwrite non-zero with non-zero - change value inside a node */
void test4() 
{
  RangeList rl;

  rl.insert_value(  18, 1 );
  rl.insert_value(  0, 1 );
  rl.insert_value(  7, 1 );
  rl.insert_value(  12, 1 );
  rl.insert_value(  5, 1 );
  rl.insert_value(  10, 1 );
  rl.insert_value(  2, 1 );
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "write 2 to position 2\n" );
  rl.insert_value(  2, 2 );
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );
  
  printf( "write 12 to position 12\n" );
  rl.insert_value(  12, 12 );
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "write 7 to position 7\n" );
  rl.insert_value(  7, 7 );
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "write 18 to position 18\n" );
  rl.insert_value(  18, 18 );
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  printf( "write -1 to position 0\n" );
  rl.insert_value(  0, -1 );
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

}

/* stress test for inserts and deletes.
 * Given indices 0 2 4 6 8 .... , create a list of values at those indices
 * by inserting values in all possible orders (permutations) 
 * and for each of those delete in all possible orders. 
 * There are n! possible permutations of n positions,
 * so runtime is n! * n! lists created and destroyed:
 * n = 5 => 120*120=14400
 */
void test5() 
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
  RangeList rl;

  for ( p1 = 0; p1 < 120; ++p1 ) {
    for ( p2 = 0; p2 < 120; ++p2 ) {
      /* insert 5 elements in order p1 */
      for ( i = 0; i < 5; ++i ) {
        rl.insert_value(  permutations[ p1 ][ i ] , permutations[ p1 ][ i ] ); /* insert */
      }
      rl.print_raw(); /* should be  1 3 5 7 9 (at positions 1 3 5 7 9 */
      printf( "\n" );
      /* delete 5 elements in order p2 */
      for ( i = 0; i < 5; ++i ) {
        rl.insert_value(  permutations[ p2 ][ i ] , 0 ); /* delete */
      }
      rl.print_raw(); /* should be empty */
      printf( "\n" );
    }
  }
}

// C++ related - copy ctor
void test6() 
{
  RangeList rl;

  rl.insert_value(  18, 1 );
  rl.insert_value(  0, 1 );
  rl.insert_value(  7, 1 );
  rl.insert_value(  12, 1 );
  rl.insert_value(  5, 1 );
  rl.insert_value(  10, 1 );
  rl.insert_value(  2, 1 );
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  RangeList rl_copy( rl );
  rl_copy.print_raw();
  printf( "\n" );
  rl_copy.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  // now change the original
  rl.insert_value(  18, 2 );
  rl.insert_value(  0, 2 );
  rl.insert_value(  7, 2 );
  rl.insert_value(  12, 0 );
  rl.insert_value(  5, 0 );
  
  // should change
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  // should NOT change
  rl_copy.print_raw();
  printf( "\n" );
  rl_copy.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  // now change the copy
  rl_copy.insert_value(  12, 5 );
  rl_copy.insert_value(  6, 3 );
  rl_copy.insert_value(  5, 6 );
  rl_copy.insert_value(  2, 4 );
  
  // should NOT change
  rl.print_raw();
  printf( "\n" );
  rl.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  // should change
  rl_copy.print_raw();
  printf( "\n" );
  rl_copy.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );
  
}

// C++ related - assignment
void test7() 
{
  RangeList rl1;

  // 1 [ 0, 1 ) --> 1 [ 2, 3 ) --> 1 [ 5, 6 ) --> 1 [ 7, 8 ) --> 1 [ 10, 11 ) --> 1 [ 12, 13 ) --> 1 [ 18, 19 ) --> NULL
  rl1.insert_value(  18, 1 );
  rl1.insert_value(  0, 1 );
  rl1.insert_value(  7, 1 );
  rl1.insert_value(  12, 1 );
  rl1.insert_value(  5, 1 );
  rl1.insert_value(  10, 1 );
  rl1.insert_value(  2, 1 );
  rl1.print_raw();
  printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  RangeList rl2;
  rl2.insert_value(  17, 2 );
  rl2.insert_value(  2, 2 );
  rl2.insert_value(  12, 2 );
  rl2.insert_value(  8, 2 );
  rl2.insert_value(  3, 2 );
  rl2.print_raw();
  printf( "\n" );
  rl2.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

// now assign 
  rl1 = rl2;

  // now change the first
  rl1.insert_value(  18, 3 );
  rl1.insert_value(  0, 3 );
  rl1.insert_value(  7, 3 );
  rl1.insert_value(  12, 3 );
  std::cout << "---------------\n";
  rl1.insert_value(  5, 0 );
  
  // should change
  rl1.print_raw();
  printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  // should NOT change
  rl2.print_raw();
  printf( "\n" );
  rl2.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  // now change the second
  rl2.insert_value(  12, 4 );
  rl2.insert_value(  6, 5 );
  rl2.insert_value(  5, 4 );
  rl2.insert_value(  2, 4 );
  
  // should NOT change
  rl1.print_raw();
  printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  // should change
  rl2.print_raw();
  printf( "\n" );
  rl2.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );
}

// Proxy related 
void test8() 
{
  RangeList rl1;

  rl1.insert_value(  18, 1 );
  rl1.insert_value(  0, 1 );
  rl1.insert_value(  7, 1 );
  rl1.insert_value(  12, 1 );
  rl1.insert_value(  5, 1 );
  rl1.insert_value(  10, 1 );
  rl1.insert_value(  2, 1 );

  // read existing (non-zero) position
  std::cout << "value at position " << 5 << " is " << rl1[5] << std::endl;

  // read non-existing (zero) position
  std::cout << "value at position " << 6 << " is " << rl1[6] << std::endl;
}

void test9() 
{
  RangeList rl1;

  rl1.insert_value(  18, 1 );
  rl1.insert_value(  0, 1 );
  rl1.insert_value(  7, 1 );
  rl1.insert_value(  12, 1 );
  rl1.insert_value(  5, 1 );
  rl1.insert_value(  10, 1 );
  rl1.insert_value(  2, 1 );

  // use Proxy to modify positions
  // modify non-zero value to another non-zero value
  rl1[5] = 2;
  rl1.print_raw(); printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK ); printf( "\n" );
  // modify zero value to a non-zero value
  rl1[16] = 2;
  rl1.print_raw(); printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK ); printf( "\n" );
  // modify non-zero value to a zero value
  rl1[10] = 0;
  rl1.print_raw(); printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK ); printf( "\n" );
}

void test10() 
{
  RangeList rl1;

  rl1.insert_value(  18, 1 );
  rl1.insert_value(  0, 1 );
  rl1.insert_value(  7, 1 );
  rl1.insert_value(  12, 1 );
  rl1.insert_value(  5, 1 );
  rl1.insert_value(  10, 1 );
  rl1.insert_value(  2, 1 );

  // use Proxy to modify positions
  // modify non-zero value to another non-zero value
  rl1[5] = 2;
  rl1.print_raw(); printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK ); printf( "\n" );
  // modify zero value to a non-zero value
  rl1[16] = 2;
  rl1.print_raw(); printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK ); printf( "\n" );
  // modify non-zero value to a zero value
  rl1[10] = 0;
  rl1.print_raw(); printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK ); printf( "\n" );

  // using proxy on both sides of assignments
  rl1[2] = rl1[2] + rl1[5];       // no need to implement NodeProxy::operator+, just implement a conversion NodeProxy -> int 
  rl1.print_raw(); printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK ); printf( "\n" );

  rl1[7] += rl1[5];       // requires NodeProxy::operator+=.
  rl1.print_raw(); printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK ); printf( "\n" );

  rl1[5] += 6;       // requires NodeProxy::operator+=.
  rl1.print_raw(); printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK ); printf( "\n" );

  rl1[5] *= 6;       // requires NodeProxy::operator*=.
  rl1.print_raw(); printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK ); printf( "\n" );

  // I will not use operator-= and/or operator/=
}

// scalar product
void test11() 
{
  RangeList rl1;

  rl1.insert_value(  0, 1 );
  rl1.insert_value(  18, 1 );
  rl1.insert_value(  7, 1 );
  rl1.insert_value(  12, 1 );
  rl1.insert_value(  5, 1 );
  rl1.insert_value(  10, 1 );
  rl1.insert_value(  2, 1 );
  rl1.print_raw();
  printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );
  
  RangeList rl2;

  rl2.insert_value(  0, 2 );
  rl2.insert_value(  17, 2 );
  rl2.insert_value(  7, 2 );
  rl2.insert_value(  12, 2 );
  rl2.insert_value(  5, 2 );
  rl2.insert_value(  8, 2 );
  rl2.insert_value(  3, 2 );
  rl2.print_raw();
  printf( "\n" );
  rl2.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );

  int scalar_product = rl1 * rl2;
  std::cout << "scalar_product  = " << scalar_product << std::endl;
}

void test12() 
{
  RangeList rl1;
  rl1.insert_nonoverlapping_range( 1, 300, 1 );
  RangeList rl2;
  rl2.insert_nonoverlapping_range( 100, 200, 1 );
  int scalar_product = rl1 * rl2;
  std::cout << "scalar_product  = " << scalar_product << std::endl;
}

void test13() 
{
  RangeList rl1;
  rl1.insert_nonoverlapping_range( 1,  15, 1 );
  rl1.insert_nonoverlapping_range( 20, 25, 1 );
  rl1.insert_nonoverlapping_range( 50, 65, 1 );

  RangeList rl2;
  rl2.insert_nonoverlapping_range( 5, 20, 1 );
  rl2.insert_nonoverlapping_range( 30,35, 1 );
  rl2.insert_nonoverlapping_range( 40,55, 1 );

  int scalar_product = rl1 * rl2;
  std::cout << "scalar_product  = " << scalar_product << std::endl;
}

// vector addition
void test14() 
{
  RangeList rl1;

  rl1.insert_value(  0, 1 );
  rl1.insert_value(  18, 1 );
  rl1.insert_value(  7, 1 );
  rl1.insert_value(  12, 1 );
  rl1.insert_value(  5, 1 );
  rl1.insert_value(  10, 1 );
  rl1.insert_value(  2, 1 );
  rl1.print_raw();
  printf( "\n" );
  rl1.printf_elements( "%3i", 20, SANITY_CHECK );
  printf( "\n" );
  
  RangeList rl2;

  rl2.insert_value(  0, 2 );
  rl2.insert_value(  17, 2 );
  rl2.insert_value(  7, 2 );
  rl2.insert_value(  12, 2 );
  rl2.insert_value(  5, 2 );
  rl2.insert_value(  8, 2 );
  rl2.insert_value(  3, 2 );
  rl2.print_raw(); printf( "\n" );
  rl2.printf_elements( "%3i", 20, SANITY_CHECK ); printf( "\n" );

  RangeList sum = rl1 + rl2;
  sum.print_raw(); printf( "\n" );
  sum.printf_elements( "%3i", 20, SANITY_CHECK ); printf( "\n" );
}

void test15() 
{
  RangeList rl1;
  rl1.insert_nonoverlapping_range( 1, 300, 1 );
  RangeList rl2;
  rl2.insert_nonoverlapping_range( 100, 200, 1 );

  RangeList sum = rl1 + rl2;
  sum.print_raw(); printf( "\n" );
  sum.printf_elements( "%4i", 300, SANITY_CHECK ); printf( "\n" );
}

void test16() 
{
  RangeList rl1;
  rl1.insert_nonoverlapping_range( 1,  3, 1 );
  rl1.insert_nonoverlapping_range( 6,  7, 1 );

  RangeList rl2;
  rl2.insert_nonoverlapping_range( 2, 4, 1 );

  rl1.printf_elements( "%3i", 10, SANITY_CHECK, false ); printf( "\n" );
  rl2.printf_elements( "%3i", 10, SANITY_CHECK, false ); printf( "\n" );
  printf( "----------------------------------------------\n" );
  RangeList sum = rl1 + rl2;
  sum.printf_elements( "%3i", 10, SANITY_CHECK, false ); printf( "\n" );
}

void test17() 
{
  RangeList rl1;
  rl1.insert_nonoverlapping_range( 1,  15, 1 );
  rl1.insert_nonoverlapping_range( 20, 25, 1 );
  rl1.insert_nonoverlapping_range( 50, 65, 1 );

  RangeList rl2;
  rl2.insert_nonoverlapping_range( 5, 20, 1 );
  rl2.insert_nonoverlapping_range( 30,35, 1 );
  rl2.insert_nonoverlapping_range( 40,55, 1 );

  RangeList sum = rl1 + rl2;
  rl1.printf_elements( "%3i", 80, SANITY_CHECK, false ); printf( "\n" );
  rl2.printf_elements( "%3i", 80, SANITY_CHECK, false ); printf( "\n" );
  printf( "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n" );
  sum.printf_elements( "%3i", 80, 1, false ); printf( "\n" );
}

void (*pTests[])(void) = {
  test0, test1, test2, test3, test4, test4, test6, test7, test8, test9,
  test10, test11, test12, test13, test14, test15, test16, test17
}; 

int main () 
{
  for(int i = 0;i<=17;++i)
  {
    pTests[i]();
  }
  return 0;
}
