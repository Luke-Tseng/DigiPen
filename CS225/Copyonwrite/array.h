#ifndef ARRAY_H
#define ARRAY_H
#include "abstract-element.h"
#include "element-factory.h"
#include <iostream>

namespace DigiPen {

/*
 * Class Array implements a simple array-like data structure.
 * Objects inside the Array have to be derived from AbstractElement.
 */
	class Array {
		public:
			Array(int * array, unsigned int _size, 
					const ElementFactory* _pElementFactory
					);
			Array(const Array &rhs);
			~Array();
			Array &operator=(const Array &rhs);
			int Get(unsigned int pos) const;
			void Set(int id, int pos, int value);
			void Print() const;
		private:
			void DeleteData();
			Array DeepCopy();
			AbstractElement** data;
			unsigned int size;
			ElementFactory const* pElementFactory;
			int* ref_count;
	};
}
#endif
