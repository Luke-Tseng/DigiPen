#include "rangelist.h"
#include <stdio.h>  /* printf */
#include <stdlib.h> /* NULL/malloc/free */

#define xDEBUG

/* helper function, prototyped in the implementation file, so NOT available
   in the main */
void dump_node(struct RangeNode *p);
static struct RangeNode *createNode(int position, int value, struct RangeNode* next);
/* your implementations: insert_value, get, delete */

/* insert a single value */
int insert_value(struct RangeNode **ppHead, int position, int value)
{
  struct RangeNode *walker = *ppHead;
  struct RangeNode *next = *ppHead;
  struct RangeNode *node = NULL;
  if (walker == NULL)
  {
    node = createNode(position, value, NULL);
    *ppHead = node;
    return 0;
  }
  if (walker->next == NULL)
  {
    if (walker->begin > position)
    {
      node = createNode(position, value, walker);
      *ppHead = node;
      return 0;
    }
  }
  if (position == 0 || position <= walker->begin)
  {
    if (value == 0)
    {
      *ppHead = walker->next;
      free(walker);
      return 0;
    }
    else
    {
      if (walker->value != 0 && walker->begin == 0)
      {
        walker->value = value;
      }
      else
      {
        node = createNode(position, value, walker);
        *ppHead = node;
      }
      return 0;
    }
  }
  if (value != 0)
  {

    while (next != NULL)
    {
      next = next->next;
      if (next == NULL)
        break;
      if (next->begin <= position && next->end > position)
      {
        next->value = value;
        return 0;
      }
      if (next->begin > position)
      {
        break;
      }
      walker = walker->next;
    }

    node = createNode(position, value, next);
    walker->next = node;
    return 0;
  }
  else
  {
    while (next != NULL)
    {
      if (walker->next == NULL)
      {
        return 0;
      }
      next = walker->next;

      if (next->begin <= position && next->end > position)
      {
        walker->next = next->next;
        free(next);
        return 0;
      }
      walker = walker->next;
    }
  }
  return 0;
}

/* return value at position (index) */
int get(struct RangeNode const *p, int position)
{
  while (p != NULL)
  {
    if (position == p->begin)
    {
      return p->value;
    }
    p = p->next;
  }
  return 0;
}

/* deallocate */
void delete (struct RangeNode *p)
{
  struct RangeNode *temp;
  while (p != NULL)
  {
    temp = p;
    p = p->next;
    free(temp);
  }
}

/* implementations - DO NOT MODIFY*/
void dump_node(struct RangeNode *p)
{
  printf("+--------------------------\n");
  printf("| %p | %i [ %i, %i ) --> %p\n", (void *)p, p->value, p->begin, p->end,
         (void *)p->next);
  printf("+--------------------------\n");
}

void printf_elements(struct RangeNode const *p, char *format, int dimension,
                     int do_sanity_check)
{
  int i, last_pos = 0;
  typedef int boolean;
  boolean zeroes = 0;
  boolean non_merged = 0;
  boolean overlapping_ranges = 0;
  /* indices */
  for (i = 0; i < dimension; ++i)
  {
    printf(format, i);
  }
  printf("\n");
  /* data */
  while (p)
  {
    for (i = last_pos; i < p->begin; ++i)
    {
      printf(format, 0);
    } /* print leading zeros */
    if (p->value == 0)
    {
      zeroes = 1;
    } /* sanity check */
    for (i = p->begin; i < p->end; ++i)
    {
      printf(format, p->value);
    } /* print value */
    last_pos = p->end;
    if (p->next && p->end > p->next->begin)
    {
      overlapping_ranges = 1;
    } /* sanity check */
    if (p->next && p->end == p->next->begin && p->value == p->next->value)
    {
      non_merged = 1;
    } /* sanity check */
    p = p->next;
  }
  for (i = last_pos; i < dimension; ++i)
  {
    printf(format, 0);
  }

  /* sanity reporting */
  if (do_sanity_check && zeroes)
  {
    printf("zero values should not be stored\n");
  }
  if (do_sanity_check && overlapping_ranges)
  {
    printf("overlapping ranges\n");
  }
  if (do_sanity_check && non_merged)
  {
    printf("ranges with the same value and next to each other - should be "
           "merged\n");
  }
}

void print_raw(struct RangeNode const *p)
{
  while (p)
  {
    printf("%i [ %i, %i ) --> ", p->value, p->begin, p->end);
    p = p->next;
  }
  printf("NULL");
}

int insert_nonoverlapping_range(struct RangeNode **ppHead, int begin, int end,
                                int value)
{
  struct RangeNode *new_node =
      (struct RangeNode *)malloc(sizeof(struct RangeNode));
  if (new_node == NULL)
  {
    return 1;
  }

  new_node->begin = begin;
  new_node->end = end;
  new_node->value = value;

  /* main logic */
  if (!*ppHead ||
      end < (*ppHead)->begin)
  { /* empty or in-front (short circuit) */
    new_node->next = *ppHead;
    *ppHead = new_node;
    return 0;
  }
  else
  { /* insert in the middle or tail */
    struct RangeNode *p = *ppHead;
    while (p->next && p->next->end <= begin)
    { /* find the right place */
      p = p->next;
    }

    if (!p->next)
    { /* insert in the end */
      new_node->next = NULL;
      p->next = new_node; /* make tail */
      return 0;
    }
    else
    { /* insert between p and p->next */
      new_node->next = p->next;
      p->next = new_node;
    }
  }
  return 0;
}

static struct RangeNode *createNode(int position, int value, struct RangeNode* next) {
  struct RangeNode *node = malloc(sizeof(struct RangeNode));
  node->next = next;
  node->begin = position;
  node->end = position + 1;
  node->value = value;
  return node;
}