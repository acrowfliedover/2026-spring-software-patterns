#include "XMLBuilder.H"
#include "Document.H"
#include "Element.H"
#include "Text.H"
#include "Attr.H"

DOMBuilder::DOMBuilder()
{
	document = new Document_Impl();
	currentNode = document;
}

void DOMBuilder::addChild(const std::string &name, const std::string &value, short nodeType)
{
	switch (nodeType)
	{
	case dom::Node::ELEMENT_NODE:
	{
		dom::Element *element = document->createElement(name);
		currentNode->appendChild(element);
		break;
	}
	case dom::Node::TEXT_NODE:
	{
		dom::Text *text = document->createTextNode(name);
		currentNode->appendChild(text);
		break;
	}
	case dom::Node::ATTRIBUTE_NODE:
	{
		dom::Attr *attr = document->createAttribute(name);
		attr->setValue(value);
		dom::Element *element = dynamic_cast<dom::Element *>(currentNode);
		if (element)
			element->setAttributeNode(attr);
		break;
	}
	}
}

void DOMBuilder::goToChild(int index)
{
	if (!currentNode->hasChildNodes())
		return;

	if (index < 0)
	{
		currentNode = currentNode->getLastChild();
	}
	else
	{
		dom::NodeList::iterator it = currentNode->getChildNodes()->begin();
		for (int i = 0; i < index && it != currentNode->getChildNodes()->end(); i++, it++)
			;
		if (it != currentNode->getChildNodes()->end())
			currentNode = *it;
	}
}

void DOMBuilder::goToParent()
{
	if (currentNode->getParentNode() != 0)
		currentNode = currentNode->getParentNode();
}

dom::Document *DOMBuilder::getResult()
{
	return document;
}
