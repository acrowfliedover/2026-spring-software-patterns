#ifndef STDOUTOBSERVER_H
#define STDOUTOBSERVER_H

#include "Observer.H"

namespace dom
{
	class Node;
};

class StdOutObserver : public Observer
{
public:
	virtual void		update(std::shared_ptr<dom::Node> container, short targetType, const std::string & target);
};

#endif // STDOUTOBSERVER_H
