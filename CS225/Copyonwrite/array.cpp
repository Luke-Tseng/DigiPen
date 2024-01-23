#include "array.h"
// if Factory is used correctly, there will be no more includes

DigiPen::Array::Array(int *array,
					  unsigned int _size,
					  const DigiPen::ElementFactory *_pElementFactory)
	: data(new AbstractElement *[_size]),
	  size(_size),
	  pElementFactory(_pElementFactory),
	  ref_count(new int(1))
{
	for (unsigned int i = 0; i < size; ++i)
	{
		// create Element with id = 1 (that is Element1 )
		data[i] = pElementFactory->MakeElement(1, array[i]);
	}
}

DigiPen::Array::Array(const DigiPen::Array &rhs)
	: data(rhs.data),
	  size(rhs.size),
	  pElementFactory(rhs.pElementFactory),
	  ref_count(rhs.ref_count)
{
	if (ref_count)
		(*ref_count)++;
}

DigiPen::Array::~Array()
{
	DeleteData();
}

DigiPen::Array &DigiPen::Array::operator=(const DigiPen::Array &rhs)
{
	if (this != &rhs && ref_count != rhs.ref_count)
	{
		if (*rhs.ref_count == 1)
		{
			DeleteData();
		}
		size = rhs.size;
		pElementFactory = rhs.pElementFactory;
		ref_count = rhs.ref_count;
		if (ref_count )
			(*ref_count)++;
		data = rhs.data;
	}
	return *this;
}

int DigiPen::Array::Get(unsigned int pos) const
{
	return data[pos]->Get();
}

void DigiPen::Array::Set(int id, int pos, int value)
{
	if (*ref_count > 1)
	{
		(*ref_count)--;
		DigiPen::Array copy = DeepCopy();
		delete copy.data[pos];
		copy.data[pos] = pElementFactory->MakeElement(id, value); // make sure is used
	}
	else
	{
		delete data[pos];
		data[pos] = pElementFactory->MakeElement(id, value); // make sure is used
	}
}

void DigiPen::Array::Print() const
{
	for (unsigned int i = 0; i < size; ++i)
		data[i]->Print();
	std::cout << std::endl;
}

void DigiPen::Array::DeleteData()
{
	if (ref_count)
	{
		(*ref_count)--;
		if (*ref_count == 0 )
		{
			for (unsigned int i = 0; i < size; ++i)
			{
				delete data[i];
			}
			delete[] data;
			delete ref_count;
		}
	}
}

DigiPen::Array DigiPen::Array::DeepCopy()
{
	AbstractElement **newData = new AbstractElement *[size];
	for (unsigned int i = 0; i < size; ++i)
	{
		newData[i] = data[i]->Clone();
	}
	ref_count = new int(1);
	data = newData;
	return *this;
}
