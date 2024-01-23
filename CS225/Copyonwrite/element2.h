#ifndef ELEMENT2_H
#define ELEMENT2_H
#include <iostream>
#include "abstract-element.h"

namespace DigiPen
{
  class Element2 : public AbstractElement
  {
  public:
    Element2(int _val);
    Element2(const Element2 &rhs);
    ~Element2();
    Element2 &operator=(const Element2 &rhs);
    Element2 *Clone() const;
    int Get() const;
    void Set(int new_val);
    void Print() const;
    static int GetAlive() { return Element2::alive; };
    static int GetTotal() { return Element2::total; };

  private:
    int *p_val;
    static int alive;
    static int total;
  };
}
#endif
