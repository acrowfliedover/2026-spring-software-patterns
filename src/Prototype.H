#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include <memory>

namespace dom
{
	class Prototype
	{
	public:
		virtual ~Prototype() {}

		virtual std::shared_ptr<Prototype>	clone(void)	= 0;
	};
};

#endif // PROTOTYPE_H
