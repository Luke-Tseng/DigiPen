#include "element2.h"

int DigiPen::Element2::alive = 0;
int DigiPen::Element2::total = 0;

DigiPen::Element2::Element2(int _val) : p_val(new int(_val))
{
	alive++;
	total++;
}

DigiPen::Element2::Element2(const Element2 &rhs)
	: AbstractElement(), p_val(new int(*rhs.p_val))
{
}

DigiPen::Element2::~Element2()
{
	alive--;
	delete p_val;
}

DigiPen::Element2 &DigiPen::Element2::operator=(const Element2 &rhs)
{
	delete p_val;
	p_val = new int(*rhs.p_val);
	return *this;
}

DigiPen::Element2 *DigiPen::Element2::Clone() const
{
	alive++;
	total++;
	return new Element2(*this);
}

int DigiPen::Element2::Get() const
{
	return *p_val;
}

void DigiPen::Element2::Print() const
{
	std::cout << "(" << *p_val << ") ";
}

void DigiPen::Element2::Set(int new_val)
{
	*p_val = new_val;
}
