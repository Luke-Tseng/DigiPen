#ifndef ABSTRACTELEMENT_H
#define ABSTRACTELEMENT_H

namespace DigiPen
{
	class AbstractElement
	{
	public:
		virtual ~AbstractElement() {}
		virtual AbstractElement *Clone() const = 0;
		virtual int Get() const = 0;
		virtual void Set(int new_val) = 0;
		virtual void Print() const = 0;
	};
}
#endif
