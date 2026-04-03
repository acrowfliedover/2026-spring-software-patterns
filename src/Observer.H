#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>

namespace dom
{
	class Node;
};

class Observer
{
public:
	virtual void	update(std::shared_ptr<dom::Node> container, short targetType, const std::string & target) = 0;
};

#endif // OBSERVER_H
