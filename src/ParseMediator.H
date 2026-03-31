#ifndef PARSEMEDIATOR_H
#define PARSEMEDIATOR_H

#include <map>
#include <vector>
#include <memory>
#include "ParseObserver.H"

class Builder;

struct ParseState
{
	int	elementCount;
	int	attributeCount;
	int	textCount;
	int	currentDepth;

	ParseState() : elementCount(0), attributeCount(0), textCount(0), currentDepth(0) {}
};

class ParseMediator
{
private:
	std::map<const Builder *, std::vector<std::shared_ptr<ParseObserver>>>	observersBySubject;
	std::map<const Builder *, ParseState>					stateBySubject;

public:
	void	addObserver(const Builder * subject, std::shared_ptr<ParseObserver> observer);
	void	removeObserver(const Builder * subject, std::shared_ptr<ParseObserver> observer);
	void	update(const Builder * subject, const ParseEvent & event);
	int	getElementCount(const Builder * subject) const;
	int	getAttributeCount(const Builder * subject) const;
	int	getTextCount(const Builder * subject) const;
	int	getCurrentDepth(const Builder * subject) const;
};

#endif // PARSEMEDIATOR_H
