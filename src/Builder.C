#include "Builder.H"
#include <ctype.h>
#include "Document.H"
#include "Element.H"
#include "Attr.H"
#include "ParseMediator.H"
#include "Text.H"

Builder::Builder(std::shared_ptr<dom::Document> _factory, std::shared_ptr<ParseMediator> _changeManager)
	: factory(_factory),
	  currentElement(0),
	  currentAttr(0),
	  changeManager(_changeManager ? _changeManager : std::make_shared<ParseMediator>()),
	  depth(0)
{
}

void Builder::attach(std::shared_ptr<ParseObserver> observer)
{
	changeManager->addObserver(this, observer);
}

void Builder::detach(std::shared_ptr<ParseObserver> observer)
{
	changeManager->removeObserver(this, observer);
}

void Builder::notify(const ParseEvent & event)
{
	changeManager->update(this, event);
}

void Builder::addValue(const std::string & text)
{
	std::string trimmed = trim(text);
	elementStack.top()->appendChild(factory->createTextNode(trimmed));
	notify(ParseEvent(ParseEvent::TEXT_ADDED, trimmed, depth));
}

void Builder::confirmElement(const std::string & tag)
{
	notify(ParseEvent(ParseEvent::ELEMENT_CONFIRMED, trim(tag), depth));
}

void Builder::createAttribute(const std::string & attribute)
{
	std::string	trimmed	= trim(attribute);
	currentAttr	= factory->createAttribute(std::string(trimmed, 0, trimmed.size() - 1));
	notify(ParseEvent(ParseEvent::ATTRIBUTE_CREATED, std::string(trimmed, 0, trimmed.size() - 1), depth));
}

void Builder::createElement(const std::string & tag)
{
	std::string trimmed = trim(tag);
	currentElement	= factory->createElement(trimmed);

	if (elementStack.size() == 0)
		factory->appendChild(currentElement);
	else
		elementStack.top()->appendChild(currentElement);

	notify(ParseEvent(ParseEvent::ELEMENT_CREATED, trimmed, depth));
}

void Builder::createProlog(void)
{
	notify(ParseEvent(ParseEvent::PROLOG_START, depth));
}

void Builder::endProlog(void)
{
	notify(ParseEvent(ParseEvent::PROLOG_END, depth));
}

void Builder::identifyProlog(const std::string & id)
{
	notify(ParseEvent(ParseEvent::PROLOG_ID, trim(id), depth));
}

bool Builder::popElement(void)
{
	currentElement	= elementStack.top();
	elementStack.pop();
	depth--;
	notify(ParseEvent(ParseEvent::ELEMENT_POPPED, depth));
	return elementStack.size() > 0;
}

void Builder::pushElement(void)
{
	if (currentElement)
	{
		elementStack.push(currentElement);
		currentElement	= 0;
		depth++;
		notify(ParseEvent(ParseEvent::ELEMENT_PUSHED, depth));
	}
}

void Builder::valueAttribute(const std::string & value)
{
	std::string	trimmed	= trim(value);
	std::string	attrValue = std::string(trimmed, 1, trimmed.size() - 2);
	currentAttr->setValue(attrValue);

	if (currentElement != 0)
		currentElement->setAttributeNode(currentAttr);

	notify(ParseEvent(ParseEvent::ATTRIBUTE_VALUE,
		currentAttr->getName() + "=\"" + attrValue + "\"", depth));
}

const std::string Builder::trim(const std::string & s) const
{
	int	start_index;
	int	stop_index;

	for (start_index = 0; start_index < s.size() && isspace(s[start_index]); start_index++);
	for (stop_index = s.size() - 1; stop_index >= start_index && isspace(s[stop_index]); stop_index--);

	return std::string(s, start_index, stop_index - start_index + 1);
}
