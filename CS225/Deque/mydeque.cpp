#include "mydeque.h"
#include <iostream>
#include <iomanip>

namespace DigiPen
{

	// implemented
	Deque::Deque() : b(0), e(0), size(0), capacity(0), array(nullptr)
	{
	}

	// implemented
	Deque::Deque(int *array_, unsigned int size_) : b(0), e(size_), size(e), capacity(size), array(new int[size])
	{
		for (int i = 0; i < e; ++i)
			array[i] = array_[i];
	}

	// implemented
	// trim capacity to size when coping
	Deque::Deque(const Deque &rhs) : b(0), e(rhs.size),
									 size(e), capacity(size),
									 array(size ? new int[size] : nullptr) // size == 0 => array = nullptr in ctor
	{
		for (int i = 0; i < e; ++i)
			array[i] = rhs[i];
	}

	Deque &Deque::operator=(Deque rhs)
	{
		if (capacity < rhs.size)
		{
			reallocate(rhs.capacity);
		}
		b = 0;
		e = rhs.size;
		size = rhs.size;
		capacity = rhs.capacity;
		for (int i = 0; i < e; ++i)
			array[i] = rhs[(rhs.b + rhs.capacity + i) % rhs.capacity];
		return *this;
	}

	Deque::~Deque()
	{
		reallocate(0);
	}

	// implemented
	int Deque::Size() const { return size; }

	// implemented
	bool Deque::Empty() const { return (size == 0); }

	void Deque::Clear()
	{
		delete[] array;
		array = nullptr;
		b = 0;
		e = 0;
		size = 0;
		capacity = 0;
	}

	// implemented
	int Deque::Capacity() const { return capacity; }

	void Deque::Push_back(int val)
	{
		if (size == capacity)
		{ // double the array size, or change size=0 to size=1
			reallocate(capacity ? capacity * 2 : 1);
		}
		array[e] = val;
		e = (e + 1) % capacity;
		++size;
	}

	int Deque::Pop_back()
	{
		if (size == capacity / 4)
		{ // half the array
			reallocate(capacity / 2);
		}

		e = (e + capacity - 1) % capacity;
		int value = array[e];
		array[e] = 0;
		size--;
		return value;
	}

	void Deque::Push_front(int val)
	{
		if (size == capacity)
		{ // double the array size, or change size=0 to size=1
			reallocate(capacity ? capacity * 2 : 1);
		}
		b = (b + capacity - 1) % capacity;
		array[b] = val;
		size++;
	}

	int Deque::Pop_front()
	{
		if (size == capacity / 4)
		{ // half the array
			reallocate(capacity / 2);
		}
		int value = array[b];
		array[b] = 0;
		b = (b + 1) % capacity;
		size--;
		return value;
	}

	int &Deque::operator[](unsigned int pos)
	{
		return array[(b + pos) % capacity];
	}

	int Deque::operator[](unsigned int pos) const
	{
		return array[(b + pos) % capacity];
	}

	void Deque::swap(Deque &other)
	{
		Deque temp = other;
		other = *this;
		*this = temp;
	}

	// if current capacity is not enough for the combined:
	// reallocate and trim, else do not reallocate at all
	Deque &Deque::operator+=(const Deque &op2)
	{
		if (size + op2.size > capacity)
		{
			reallocate(size + op2.size);
		}
		for (int i = 0; i < op2.size; ++i)
		{
			Push_back(op2[i]);
		}
		return *this;
	}

	Deque Deque::operator+(const Deque &rhs) const
	{
		Deque combine = *this;
		if (size + rhs.size > capacity)
		{
			combine.reallocate(size + rhs.size);
		}
		for (int i = 0; i < rhs.size; ++i)
		{
			combine.Push_back(rhs.array[(rhs.b + rhs.capacity + i) % rhs.capacity]);
		}
		return combine;
	}

	Deque &Deque::reverse()
	{
		for (int i = 0; i < size / 2; ++i)
			std::swap(array[i], array[size - 1 - i]);
		return *this;
	}

	Deque Deque::operator~() const
	{
		Deque newDeque = *this;
		newDeque = newDeque.reverse();
		return newDeque;
	}

	// implemented
	void Deque::Print() const
	{
		std::cout << "size = " << size << " capacity = " << capacity << std::endl;

		if (array == nullptr)
		{
			std::cout << "array=nullptr" << std::endl;
			return;
		}

		// deque may still be empty, but array is allocated, so print some data
		int width = 3;
		// line of indices
		for (int i = 0; i < capacity; ++i)
		{
			std::cout << std::setw(width) << i;
		}
		std::cout << " indices" << std::endl;
		// line of data
		for (int i = 0; i < capacity; ++i)
		{
			if ((b <= i && i < e)				//  ....b.....e.....
				|| (e < b && (b <= i || i < e)) //  ....e.....b.....
				|| (b == e && size > 0))		//  ....be.......... either empty or full - look at size
				std::cout << std::setw(width) << array[i];
			else
				std::cout << std::setw(width) << ".";
		}
		std::cout << " data" << std::endl;

		char ch1 = 'b', ch2 = 'e';
		int dist1 = b + 1, dist2 = e - b;
		if (e < b)
		{
			ch1 = 'e';
			ch2 = 'b';
			dist1 = e + 1;
			dist2 = b - e;
		}
		std::cout << std::setw(width * dist1) << ch1;
		std::cout << std::setw(width * dist2) << ch2;
		std::cout << std::endl;
	}

	// implemented
	void Deque::reallocate(int new_capacity)
	{

		// clean - dtor uses this
		if (new_capacity == 0)
		{
			delete[] array;
			array = nullptr;
			b = 0;
			e = 0;
			size = 0;
			capacity = 0;
		}
		else
		{

			// do not check whether new_capacity makes sense etc - it is caller's responsibility
			int *new_array = new int[new_capacity];
			for (int i = 0; i < size; ++i)
			{
				new_array[i] = array[(b + i) % capacity]; // new b is 0
			}
			// for (int i=size;i<capacity;++i) new_array[i]=0;
			delete[] array;
			array = new_array;
			b = 0;
			e = size;
			// same size
			capacity = new_capacity;
		}
	}

	// implemented
	std::ostream &operator<<(std::ostream &os, const Deque &d)
	{
		for (int i = 0; i < d.Size(); ++i)
			os << d[i] << " "; // this is public Deque::operator[], operator<< does not need to be friend
		return os;
	}
}
