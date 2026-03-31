#ifndef PARSEMEDIATOR_H
#define PARSEMEDIATOR_H

#include <vector>
#include <memory>
#include "ParseObserver.H"

//
// ParseMediator: Implements the GoF Mediator pattern combined with Observer.
// The Builder (Subject) notifies this Mediator via the Observer protocol.
// The Mediator tracks aggregate parsing state and coordinates redistribution
// of events to registered colleague observers.
//
class ParseMediator : public ParseObserver
{
private:
	std::vector<std::shared_ptr<ParseObserver>>	colleagues;

	int	elementCount;
	int	attributeCount;
	int	textCount;
	int	currentDepth;

public:
	ParseMediator();

	void	addObserver(std::shared_ptr<ParseObserver> observer);
	void	removeObserver(std::shared_ptr<ParseObserver> observer);

	int	getElementCount(void) const	{ return elementCount; }
	int	getAttributeCount(void) const	{ return attributeCount; }
	int	getTextCount(void) const	{ return textCount; }
	int	getCurrentDepth(void) const	{ return currentDepth; }

	virtual void	update(const ParseEvent & event);
};

#endif // PARSEMEDIATOR_H
