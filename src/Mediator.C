#include "Mediator.H"
#include "Observer.H"
#include <stdio.h>

std::multimap<Subject *, std::shared_ptr<Observer>>	Mediator::observers;

void Mediator::_register(Subject * subject, std::shared_ptr<Observer> observer)
{
	auto	range	= observers.equal_range(subject);

	for (std::multimap<Subject *, std::shared_ptr<Observer>>::iterator i = range.first; i != range.second; i++)
		if (i->second == observer)
			return;

	observers.emplace(subject, observer);
}

void Mediator::unregister(Subject * subject, std::shared_ptr<Observer> observer)
{
	auto	range	= observers.equal_range(subject);

	for (std::multimap<Subject *, std::shared_ptr<Observer>>::iterator i = range.first; i != range.second; i++)
		if (i->second == observer)
			observers.erase(i);
}

void Mediator::notify(Subject * subject,
  std::shared_ptr<dom::Node> container,
  short targetType,
  const std::string & target)
{
	auto	range	= observers.equal_range(subject);

	for (std::multimap<Subject *, std::shared_ptr<Observer>>::iterator i = range.first; i != range.second; i++)
		i->second->update(container, targetType, target);
}
