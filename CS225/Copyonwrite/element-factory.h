#ifndef ELEMENT_FACTORY_H
#define ELEMENT_FACTORY_H
#include "abstract-element.h"

namespace DigiPen {
	class ElementFactory {
		public:
			AbstractElement* MakeElement(int id,int value) const;	
	};
}
#endif
