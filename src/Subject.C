#include "Subject.H"
#include "Observer.H"
#include "Mediator.H"

void Subject::attach(std::shared_ptr<Observer> observer)
{
	Mediator()._register(this, observer);
}

void Subject::detach(std::shared_ptr<Observer> observer)
{
	Mediator().unregister(this, observer);
}

void Subject::notify(std::shared_ptr<dom::Node> container, short targetType, const std::string & target)
{
	Mediator().notify(this, container, targetType, target);
}
