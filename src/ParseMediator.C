#include "ParseMediator.H"
#include <algorithm>

ParseMediator::ParseMediator()
	: elementCount(0), attributeCount(0), textCount(0), currentDepth(0)
{
}

void ParseMediator::addObserver(std::shared_ptr<ParseObserver> observer)
{
	colleagues.push_back(observer);
}

void ParseMediator::removeObserver(std::shared_ptr<ParseObserver> observer)
{
	colleagues.erase(
		std::remove(colleagues.begin(), colleagues.end(), observer),
		colleagues.end()
	);
}

void ParseMediator::update(const ParseEvent & event)
{
	switch (event.type)
	{
	case ParseEvent::ELEMENT_CREATED:
		elementCount++;
		break;
	case ParseEvent::ATTRIBUTE_VALUE:
		attributeCount++;
		break;
	case ParseEvent::TEXT_ADDED:
		textCount++;
		break;
	case ParseEvent::ELEMENT_PUSHED:
		currentDepth++;
		break;
	case ParseEvent::ELEMENT_POPPED:
		currentDepth--;
		break;
	default:
		break;
	}

	ParseEvent enriched(event.type, event.data, currentDepth);

	for (auto & colleague : colleagues)
		colleague->update(enriched);
}
