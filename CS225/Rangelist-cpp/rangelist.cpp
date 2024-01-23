#include "rangelist.h"
#include <cstdlib>   /* new/delete */
#include <cstddef>   /* NULL */
#include <cstdio>    /* printf */
#include <algorithm> /* std::swap */
#include <iostream>

#define xDEBUG

RangeList::RangeList() : pHead(NULL)
{
}

RangeList::RangeList(const RangeList &rhs) : pHead(NULL)
{
  struct RangeNode *temp = rhs.pHead;
  while (temp != NULL)
  {
    for (int i = temp->begin; i < temp->end; ++i)
    {
      insert_value(i, temp->value);
    }
    temp = temp->next;
  }
}
RangeList::~RangeList()
{
  struct RangeNode *temp;
  while (pHead != NULL)
  {
    temp = pHead;
    pHead = pHead->next;
    delete temp;
  }
}

RangeList &RangeList::operator=(const RangeList &rhs)
{
  struct RangeNode *temp;
  while (pHead != NULL)
  {
    temp = pHead;
    pHead = pHead->next;
    delete temp;
  }
  temp = rhs.pHead;
  while (temp != NULL)
  {
    for (int i = temp->begin; i < temp->end; ++i)
    {
      this->insert_value(i, temp->value);
    }
    temp = temp->next;
  }
  return *this;
}

NodeProxy RangeList::operator[](int pos)
{
  return NodeProxy(*this, pos);
}

int RangeList::operator*(const RangeList &rhs)
{
  int result = 0;
  struct RangeNode *temp = rhs.pHead;
  while (temp != NULL)
  {
    for (int i = temp->begin; i < temp->end; ++i)
    {
      result += get(i) * rhs.get(i);
    }
    temp = temp->next;
  }
  return result;
}

RangeList RangeList::operator+(const RangeList &rhs)
{
  RangeList rl;
  struct RangeNode *temp = rhs.pHead;
  struct RangeNode *ptemp = pHead;
  while (temp != NULL)
  {
    for (int i = temp->begin; i < temp->end; ++i)
    {
      rl.insert_value(i, get(i) + rhs.get(i));
    }
    temp = temp->next;
  }
  while (ptemp != NULL)
  {
    for (int i = ptemp->begin; i < ptemp->end; ++i)
    {
      rl.insert_value(i, get(i) + rhs.get(i));
    }
    ptemp = ptemp->next;
  }
  return rl;
}

void RangeList::RangeNode::dump_node() const
{
  printf("+--------------------------\n");
  printf("| %p | %i [ %i, %i ) --> %p\n", reinterpret_cast<void const *>(this), value, begin, end, reinterpret_cast<void const *>(next));
  printf("+--------------------------\n");
}

int RangeList::get(int position) const
{
  struct RangeNode const *p = pHead;
  while (p != NULL)
  {
    for (int i = p->begin; i < p->end; ++i)
    {
      if (position == i)
      {
        return p->value;
      }
    }
    p = p->next;
  }
  return 0;
}

int RangeList::insert_value(int position, int value)
{
  struct RangeNode *walker = pHead;
  struct RangeNode *next = pHead;
  struct RangeNode *node = NULL;
  if (walker == NULL)
  {

    node = createNode(position, value, NULL);
    pHead = node;
    return 0;
  }
  if (walker->next == NULL)
  {
    if (walker->begin > position)
    {
      node = createNode(position, value, walker);
      pHead = node;
      return 0;
    }
  }

  if (position == 0 || position <= walker->begin)
  {
    if (value == 0)
    {
      pHead = walker->next;
      delete walker;
      return 0;
    }
    if (walker->begin == position && walker->value != value)
    {
      if (walker->end-1 == position)
      {
        walker->value = value;
      }
      else
      {
        node = createNode(position, value, walker);
        pHead = node;
        walker->begin++;
      }
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
        pHead = node;
      }
      return 0;
    }
  }
  if (value != 0)
  {
    while (next != NULL)
    {
      if (walker->begin == position && walker->value != value)
      {
        if (walker->end-1 == position)
        {
          walker->value = value;
        }
        else
        {
          node = createNode(position, value, next);
          node->next = walker;
          walker->begin++;
        }
        return 0;
      }
      if (walker->end == position && walker->value == value)
      {
        walker->end++;
        return 0;
      }
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
        delete next;
        return 0;
      }
      walker = walker->next;
    }
  }
  return 0;
}

int RangeList::insert_nonoverlapping_range(int begin, int end, int value)
{
  RangeNode **ppHead = &pHead;

  /* main logic */
  if (!*ppHead || end < (*ppHead)->begin)
  { /* empty or in-front (short circuit) */
    // front next to old head same value
    if (*ppHead && (*ppHead)->value == value && end == (*ppHead)->begin)
    {
      (*ppHead)->begin = begin;
    }
    else
    {
      RangeNode *old_head = *ppHead;
      *ppHead = new RangeNode(begin, end, value);
      (*ppHead)->next = old_head;
    }
    return 0;
  }
  else
  { /* insert in the middle or tail */
    RangeNode *p = *ppHead;
    while (p->next && p->next->end <= begin)
    { /* find the right place */
      p = p->next;
    }

    if (!p->next)
    { /* insert in the end */
      if (p->value == value && p->end == begin)
      {               // if inserting same value next to exising node
        p->end = end; // just change the end
      }
      else
      {
        p->next = new RangeNode(begin, end, value); /* make tail */
      }
      return 0;
    }
    else
    { /* insert between p and p->next */
      RangeNode *old_p_next = p->next;
      p->next = new RangeNode(begin, end, value);
      p->next->next = old_p_next;
    }
  }
  return 0;
}

void RangeList::printf_elements(char const *format, int dimension, bool do_sanity_check, bool indices) const
{
  RangeNode const *p = pHead;
  int i, last_pos = 0;
  bool zeroes = 0;
  bool non_merged = 0;
  bool overlapping_ranges = 0;
  /* indices */
  if (indices)
  {
    for (i = 0; i < dimension; ++i)
    {
      printf(format, i);
    }
    printf("\n");
  }
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
    printf("ranges with the same value and next to each other - should be merged\n");
  }
}

void RangeList::print_raw() const
{
  RangeNode const *p = pHead;
  while (p)
  {
    printf("%i [ %i, %i ) --> ", p->value, p->begin, p->end);
    p = p->next;
  }
  printf("NULL");
}

struct RangeList::RangeNode *RangeList::createNode(int position, int value, struct RangeNode *next)
{
  struct RangeNode *node = new struct RangeNode(position, position + 1, value);
  node->next = next;
  return node;
}