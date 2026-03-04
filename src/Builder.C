#include "Builder.H"

#include <ctype.h>
#include "Document.H"
#include "Element.H"
#include "Attr.H"
#include "Text.H"
#include "XMLTokenizer.H"

void Builder::buildFromFile(const std::string & filename)
{
	XMLTokenizer				tokenizer(filename);
	std::shared_ptr<XMLTokenizer::XMLToken>	token;

	currentElement	= 0;
	currentAttr	= 0;

	while (elementStack.size() > 0)
		elementStack.pop();

	for (;;)
	{
		token	= tokenizer.getNextToken();

		if (token->getTokenType() == XMLTokenizer::XMLToken::NULL_TOKEN)
			return;

		if (token->getTokenType() == XMLTokenizer::XMLToken::TAG_START)
		{
			std::shared_ptr<XMLTokenizer::XMLToken>	tagToken(tokenizer.getNextToken());

			if (tagToken->getTokenType() == XMLTokenizer::XMLToken::ELEMENT)
			{
				factory->appendChild(ElementProxy_Impl::createProxyElement(factory, filename, tokenizer, tagToken->getToken()));
				return;
			}
		}
	}
}

void Builder::addValue(const std::string & text)
{
	elementStack.top()->appendChild(factory->createTextNode(trim(text)));
}

void Builder::confirmElement(const std::string & tag)
{
	// Throw an exception if trim(tag) != currentElement.getTagName()
}

void Builder::createAttribute(const std::string & attribute)
{
	std::string	trimmed	= trim(attribute);
	currentAttr	= factory->createAttribute(std::string(trimmed, 0, trimmed.size() - 1));
}

void Builder::createElement(const std::string & tag)
{
	currentElement	= factory->createElement(trim(tag));

	if (elementStack.size() == 0)	// This is the root element.
		factory->appendChild(currentElement);
	else
		elementStack.top()->appendChild(currentElement);
}

void Builder::createProlog(void)
{
	// null method in this implementation
}

void Builder::endProlog(void)
{
	// null method in this implementation
}

void Builder::identifyProlog(const std::string & id)
{
	// null method in this implementation
}

bool Builder::popElement(void)
{
	currentElement	= elementStack.top();
	elementStack.pop();
	return elementStack.size() > 0;
}

void Builder::pushElement(void)
{
	if (currentElement)
	{
		elementStack.push(currentElement);
		currentElement	= 0;
	}
}

void Builder::valueAttribute(const std::string & value)
{
	std::string	trimmed	= trim(value);
	currentAttr->setValue(std::string(trimmed, 1, trimmed.size() - 2));

	if (currentElement != 0)	// Discard prolog attributes.  This implementation currently doesn't have
					// anything to do with them.
		currentElement->setAttributeNode(currentAttr);
}

const std::string Builder::trim(const std::string & s) const
{
	size_t	start_index	= 0;
	size_t	stop_index	= s.size();

	while (start_index < s.size() && isspace(static_cast<unsigned char>(s[start_index])))
		start_index++;

	while (stop_index > start_index && isspace(static_cast<unsigned char>(s[stop_index - 1])))
		stop_index--;

	if (start_index >= stop_index)
		return std::string("");

	return std::string(s, start_index, stop_index - start_index);
}
