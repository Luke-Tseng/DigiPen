#include "element1.h"

int DigiPen::Element1::alive = 0;
int DigiPen::Element1::total = 0;

DigiPen::Element1::Element1(int _val) : val(_val)
{
  alive++;
  total++;
}

DigiPen::Element1::~Element1()
{
  alive--;
}

DigiPen::Element1 *DigiPen::Element1::Clone() const
{
  alive++;
  total++;
  return new Element1(*this);
}

int DigiPen::Element1::Get() const { return val; }

void DigiPen::Element1::Set(int new_val) { val = new_val; }

void DigiPen::Element1::Print() const
{
  std::cout << "[" << Get() << "] ";
}
