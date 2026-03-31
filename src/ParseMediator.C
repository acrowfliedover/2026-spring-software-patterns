#include "ParseMediator.H"
#include <algorithm>

void ParseMediator::addObserver(const Builder * subject, std::shared_ptr<ParseObserver> observer)
{
	observersBySubject[subject].push_back(observer);
	stateBySubject[subject];
}

void ParseMediator::removeObserver(const Builder * subject, std::shared_ptr<ParseObserver> observer)
{
	std::map<const Builder *, std::vector<std::shared_ptr<ParseObserver>>>::iterator subjectObservers =
		observersBySubject.find(subject);

	if (subjectObservers == observersBySubject.end())
		return;

	subjectObservers->second.erase(
		std::remove(subjectObservers->second.begin(), subjectObservers->second.end(), observer),
		subjectObservers->second.end()
	);
}

void ParseMediator::update(const Builder * subject, const ParseEvent & event)
{
	ParseState & state = stateBySubject[subject];
	state.currentDepth = event.depth;

	switch (event.type)
	{
	case ParseEvent::ELEMENT_CREATED:
		state.elementCount++;
		break;
	case ParseEvent::ATTRIBUTE_VALUE:
		state.attributeCount++;
		break;
	case ParseEvent::TEXT_ADDED:
		state.textCount++;
		break;
	default:
		break;
	}

	std::map<const Builder *, std::vector<std::shared_ptr<ParseObserver>>>::iterator subjectObservers =
		observersBySubject.find(subject);

	if (subjectObservers == observersBySubject.end())
		return;

	for (auto & observer : subjectObservers->second)
		observer->update(event);
}

int ParseMediator::getElementCount(const Builder * subject) const
{
	std::map<const Builder *, ParseState>::const_iterator state = stateBySubject.find(subject);
	return state == stateBySubject.end() ? 0 : state->second.elementCount;
}

int ParseMediator::getAttributeCount(const Builder * subject) const
{
	std::map<const Builder *, ParseState>::const_iterator state = stateBySubject.find(subject);
	return state == stateBySubject.end() ? 0 : state->second.attributeCount;
}

int ParseMediator::getTextCount(const Builder * subject) const
{
	std::map<const Builder *, ParseState>::const_iterator state = stateBySubject.find(subject);
	return state == stateBySubject.end() ? 0 : state->second.textCount;
}

int ParseMediator::getCurrentDepth(const Builder * subject) const
{
	std::map<const Builder *, ParseState>::const_iterator state = stateBySubject.find(subject);
	return state == stateBySubject.end() ? 0 : state->second.currentDepth;
}
