#include "Element.H"
#include "Attr.H"
#include "Text.H"
#include "Document.H"
#include "XMLValidator.H"
#include "Visitor.H"

#include <iostream>

Element_Impl::Element_Impl(const std::string & tagName, dom::Document * document) : Node_Impl(tagName, dom::Node::ELEMENT_NODE),
  attributes(document)
{
	Node_Impl::document	= document;
}

Element_Impl::~Element_Impl()
{
}

const std::string &	Element_Impl::getAttribute(const std::string & name)
{
	for (dom::NodeList::iterator i = attributes.begin(); i != attributes.end(); i++)
	{
		std::shared_ptr<dom::Attr> attr(std::dynamic_pointer_cast<dom::Attr>(*i));

		if (attr->getName().compare(name) == 0)
			return attr->getValue();
	}

	static const std::string	empty_string("");
	return empty_string;
}

std::shared_ptr<dom::Attr>		Element_Impl::getAttributeNode(const std::string & name)
{
	for (dom::NodeList::iterator i = attributes.begin(); i != attributes.end(); i++)
	{
		std::shared_ptr<dom::Attr> attr(std::dynamic_pointer_cast<dom::Attr>(*i));

		if (attr->getName().compare(name) == 0)
			return attr;
	}

	return 0;
}

std::shared_ptr<dom::NodeList>		Element_Impl::getElementsByTagName(const std::string & tagName)
{
	std::shared_ptr<dom::NodeList>	nodeList(new dom::NodeList);

	for (dom::NodeList::iterator i = getChildNodes()->begin(); i != getChildNodes()->end(); i++)
	{
		std::shared_ptr<dom::Element>	element;

		if ((element = std::dynamic_pointer_cast<dom::Element>(*i)) && element->getTagName().compare(tagName)==0)
			nodeList->push_back(*i);
	}

	return nodeList;
}

const std::string &	Element_Impl::getTagName(void)
{
	return getNodeName();
}

bool			Element_Impl::hasAttribute(const std::string & name)
{
	for (dom::NodeList::iterator i = attributes.begin(); i != attributes.end(); i++)
	{
		std::shared_ptr<dom::Attr> attr(std::dynamic_pointer_cast<dom::Attr>(*i));

		if (attr->getName().compare(name) == 0)
			return true;
	}

	return false;
}

void			Element_Impl::removeAttribute(const std::string & name)
{
	for (dom::NodeList::iterator i = attributes.begin(); i != attributes.end(); i++)
	{
		std::shared_ptr<dom::Attr> attr(std::dynamic_pointer_cast<dom::Attr>(*i));

		if (attr->getName().compare(name) == 0)
		{
			attributes.erase(i);
			return;
		}
	}
}

std::shared_ptr<dom::Attr>		Element_Impl::removeAttributeNode(std::shared_ptr<dom::Attr> oldAttr)
{
	for (dom::NodeList::iterator i = attributes.begin(); i != attributes.end(); i++)
		if (*i == oldAttr)
		{
			std::shared_ptr<dom::Attr>	attribute(std::dynamic_pointer_cast<dom::Attr>(*i));
			attributes.erase(i);
			return attribute;
		}

	throw dom::DOMException(dom::DOMException::NOT_FOUND_ERR, "Attribute not found.");
}

void			Element_Impl::setAttribute(const std::string & name, const std::string & value)
{
	for (dom::NodeList::iterator i = attributes.begin(); i != attributes.end(); i++)
	{
		std::shared_ptr<dom::Attr> attr(std::dynamic_pointer_cast<dom::Attr>(*i));

		if (attr->getName().compare(name) == 0)
		{
			attr->setValue(value);
			return;
		}
	}

	std::shared_ptr<dom::Attr>
	  attribute(new Attr_Impl(name, value, dynamic_cast<Document_Impl *>(getOwnerDocument())));

	attributes.push_back(attribute);
	std::dynamic_pointer_cast<Node_Impl>(std::dynamic_pointer_cast<Node>(attribute))->setParent(this);
}

std::shared_ptr<dom::Attr>		Element_Impl::setAttributeNode(std::shared_ptr<dom::Attr> newAttr)
{
	if (newAttr->getOwnerDocument() != getOwnerDocument())
		throw dom::DOMException(dom::DOMException::WRONG_DOCUMENT_ERR, "Attribute not created by this document.");

	if (newAttr->getParentNode() != 0)
		throw dom::DOMException(dom::DOMException::INUSE_ATTRIBUTE_ERR, "Attribute in use by other element.");

	std::shared_ptr<dom::Attr>	oldAttribute;

	for (dom::NodeList::iterator i = attributes.begin(); i != attributes.end(); i++)
		if (std::dynamic_pointer_cast<dom::Attr>(*i)->getName().compare(newAttr->getName()) == 0)
		{
			oldAttribute	= std::dynamic_pointer_cast<dom::Attr>(*i);
			attributes.erase(i);
			break;
		}

	std::dynamic_pointer_cast<Node_Impl>(std::dynamic_pointer_cast<Node>(newAttr))->setParent(this);
	attributes.push_back(newAttr);
	return oldAttribute;
}

void Element_Impl::accept(dom::Visitor & visitor)
{
	visitor.visit(this);
}

std::shared_ptr<dom::Node> Element_Impl::cloneNode(bool deep)
{
	std::shared_ptr<dom::Element>	element(new Element_Impl(getTagName(), getOwnerDocument()));

	if (deep)
	{
		dom::NodeList *	children	= getChildNodes();

		for (dom::NodeList::iterator i = children->begin(); i != children->end(); i++)
			element->appendChild((*i)->cloneNode(deep));

		for (dom::NamedNodeMap::iterator i = attributes.begin(); i != attributes.end(); i++)
			element->setAttributeNode(std::dynamic_pointer_cast<dom::Attr>((*i)->cloneNode(deep)));
	}

	return element;
}

std::shared_ptr<dom::Node> ElementValidator::cloneNode(bool deep)
{
	return
	 std::shared_ptr<ElementValidator>(new ElementValidator(std::dynamic_pointer_cast<dom::Element>(component->cloneNode(deep)),
	  schemaElement));
}

std::shared_ptr<dom::Node> ElementProxy::cloneNode(bool deep)
{
	std::shared_ptr<ElementProxy>
	  temp(new ElementProxy(std::dynamic_pointer_cast<dom::Element>(realSubject->cloneNode(deep)), director));
	temp->realize();

	return temp;
}

ElementValidator::ElementValidator(std::shared_ptr<dom::Element> _component, std::shared_ptr<XMLValidator> xmlValidator) :
  Node_Impl("", dom::Node::ELEMENT_NODE),
  component(_component)
{
	schemaElement	= *xmlValidator->findSchemaElement(component->getTagName());
}

void ElementValidator::setAttribute(const std::string & name, const std::string & value)
{
	if (schemaElement == 0 || schemaElement->childIsValid(name, true))
		component->setAttribute(name, value);
	else
		throw dom::DOMException(dom::DOMException::VALIDATION_ERR, "Invalid attribute " + name + ".");
}

std::shared_ptr<dom::Attr> ElementValidator::setAttributeNode(std::shared_ptr<dom::Attr> newAttr)
{
	if (schemaElement == 0 || schemaElement->childIsValid(newAttr->getName(), true))
		return component->setAttributeNode(newAttr);

	throw dom::DOMException(dom::DOMException::VALIDATION_ERR, "Invalid attribute " + newAttr->getName() + ".");
}

std::shared_ptr<dom::Node> ElementValidator::insertBefore(std::shared_ptr<dom::Node> newChild, std::shared_ptr<dom::Node> refChild)
{
	if (schemaElement == 0 || std::dynamic_pointer_cast<dom::Text>(newChild) ||
	  schemaElement->childIsValid(newChild->getNodeName(), false))
		return component->insertBefore(newChild, refChild);
	else
		throw dom::DOMException(dom::DOMException::VALIDATION_ERR, "Invalid child node " + newChild->getNodeName() + ".");
}

std::shared_ptr<dom::Node> ElementValidator::replaceChild(std::shared_ptr<dom::Node> newChild, std::shared_ptr<dom::Node> oldChild)
{
	if (schemaElement == 0 || std::dynamic_pointer_cast<dom::Text>(newChild) ||
	  schemaElement->childIsValid(newChild->getNodeName(), false))
		return component->replaceChild(newChild, oldChild);
	else
		throw dom::DOMException(dom::DOMException::VALIDATION_ERR, "Invalid child node " + newChild->getNodeName() + ".");
}

std::shared_ptr<dom::Node> ElementValidator::appendChild(std::shared_ptr<dom::Node> newChild)
{
	if (schemaElement == 0 || std::dynamic_pointer_cast<dom::Text>(newChild) ||
	  schemaElement->childIsValid(newChild->getNodeName(), false))
		return component->appendChild(newChild);
	else
		throw dom::DOMException(dom::DOMException::VALIDATION_ERR, "Invalid child node " + newChild->getNodeName() + ".");
}

void ElementProxy::realize(void)
{
	// Realize by parsing child nodes.
	realized	= true;
}

dom::NodeList * ElementProxy::getChildNodes(void)
{
	if (!realized)
		realize();

	return realSubject->getChildNodes();
}

std::shared_ptr<dom::Node> ElementProxy::getFirstChild(void)
{
	if (!realized)
		realize();

	return realSubject->getFirstChild();
}

std::shared_ptr<dom::Node> ElementProxy::getLastChild(void)
{
	if (!realized)
		realize();

	return realSubject->getLastChild();
}

bool ElementProxy::hasChildNodes(void)
{
	if (!realized)
		realize();

	return realSubject->hasChildNodes();
}

void Element_Impl::HandleRequest(std::string & event)
{
	const std::string	eventTemplate	= getAttribute("message");

	if (eventTemplate == event)
		std::cout << "Handling event " << event << "." << std::endl;
	else if (getParentNode() != 0 && dynamic_cast<dom::Element *>(getParentNode()) != 0)
		dynamic_cast<dom::Element *>(getParentNode())->HandleRequest(event);
	else
		std::cout << "Reached root of DOM tree without handling event '" << event << "'." << std::endl;
}
