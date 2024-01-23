#include "element-factory.h"

//this is a concrete factory, so it has to know about all element classes
#include "element1.h"
#include "element2.h"

//for grading I'll use a different factory that supports 
//3d element types. The id for the element is not known.

//#include "element3.h" // the filename and is just an example

DigiPen::AbstractElement* DigiPen::ElementFactory::MakeElement(int id,int value) const {
	AbstractElement* p_element = NULL;
	switch (id) {
		case 1: p_element = new Element1(value); break;
		case 2: p_element = new Element2(value); break;
		//case ???: p_element = new Element????(value); break;
	}
	return p_element;
}
