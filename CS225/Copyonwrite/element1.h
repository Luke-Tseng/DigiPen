#ifndef ELEMENT1_H
#define ELEMENT1_H
#include <iostream>
#include "abstract-element.h"

namespace DigiPen
{
  class Element1 : public AbstractElement
  {
  public:
    Element1(int _val);
    ~Element1();
    Element1* Clone() const;
    int Get() const;
    void Set(int new_val);
    void Print() const;
    static int GetAlive() { return Element1::alive; };
    static int GetTotal() { return Element1::total; };

  private:
    int val;
    static int alive;
    static int total;
  };
}
#endif
