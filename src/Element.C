#include "Element.H"
#include "Attr.H"
#include "Text.H"
#include "Document.H"
#include "XMLValidator.H"
#include <cctype>

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

void Element_Impl::serialize(std::fstream * writer, std::shared_ptr<WhitespaceStrategy> whitespace)
{
	whitespace->prettyIndentation(writer);
	*writer << "<" << getTagName();

	int	attrCount	= 0;

	for (dom::NamedNodeMap::iterator i = getAttributes()->begin(); i != getAttributes()->end(); i++)
	{
		(*i)->serialize(writer, whitespace);
		attrCount++;
	}

	if (attrCount > 0)
		*writer << " ";

	if (getChildNodes()->size() == 0)
	{
		*writer << "/>";
		whitespace->newLine(writer);
	}
	else
	{
		*writer << ">";
		whitespace->newLine(writer);
		whitespace->incrementIndentation();

		for (dom::NodeList::iterator i = getChildNodes()->begin(); i != getChildNodes()->end(); i++)
			if (std::dynamic_pointer_cast<dom::Element>(*i)  || std::dynamic_pointer_cast<dom::Text>(*i))
				(*i)->serialize(writer, whitespace);

		whitespace->decrementIndentation();
		whitespace->prettyIndentation(writer);
		*writer << "</" << getTagName() + ">";
		whitespace->newLine(writer);
	}
}

ElementProxy_Impl::ElementProxy_Impl(const std::string & tagName, std::shared_ptr<dom::Document> documentFactory,
  const std::string & filename) :
  Element_Impl(tagName, documentFactory.get()),
  factory(documentFactory),
  sourceFilename(filename),
  hasDeferredChildren(false),
  childrenLoaded(true),
  loadingChildren(false)
{
}

ElementProxy_Impl::~ElementProxy_Impl()
{
}

std::shared_ptr<dom::Element> ElementProxy_Impl::createProxyElement(std::shared_ptr<dom::Document> documentFactory,
  const std::string & filename, XMLTokenizer & tokenizer, const std::string & tagToken)
{
	std::shared_ptr<ElementProxy_Impl>	proxy(new ElementProxy_Impl(trim(tagToken), documentFactory, filename));
	std::shared_ptr<XMLTokenizer::XMLToken>	token;

	for (;;)
	{
		token	= tokenizer.getNextToken();

		switch(token->getTokenType())
		{
		case XMLTokenizer::XMLToken::ATTRIBUTE:
		{
			std::string	attrName	= getAttributeName(token->getToken());
			std::shared_ptr<XMLTokenizer::XMLToken>	valueToken(tokenizer.getNextToken());

			if (valueToken->getTokenType() == XMLTokenizer::XMLToken::ATTRIBUTE_VALUE && attrName.size() > 0)
			{
				std::shared_ptr<dom::Attr>	attr(documentFactory->createAttribute(attrName));
				attr->setValue(getAttributeValue(valueToken->getToken()));
				proxy->setAttributeNode(attr);
			}
			break;
		}
		case XMLTokenizer::XMLToken::NULL_TAG_END:
			proxy->hasDeferredChildren	= false;
			proxy->childrenLoaded		= true;
			return proxy;
		case XMLTokenizer::XMLToken::TAG_END:
			proxy->childTokenizerState	= tokenizer.captureState();
			proxy->hasDeferredChildren	= true;
			proxy->childrenLoaded		= false;
			skipElementBody(tokenizer);
			return proxy;
		case XMLTokenizer::XMLToken::NULL_TOKEN:
			proxy->hasDeferredChildren	= false;
			proxy->childrenLoaded		= true;
			return proxy;
		default:
			break;
		}
	}
}

void ElementProxy_Impl::serialize(std::fstream * writer, std::shared_ptr<WhitespaceStrategy> whitespace)
{
	materializeChildren();
	Element_Impl::serialize(writer, whitespace);
}

std::shared_ptr<dom::NodeList> ElementProxy_Impl::getElementsByTagName(const std::string & tagName)
{
	materializeChildren();
	return Element_Impl::getElementsByTagName(tagName);
}

dom::NodeList * ElementProxy_Impl::getChildNodes(void)
{
	materializeChildren();
	return Node_Impl::getChildNodes();
}

std::shared_ptr<dom::Node> ElementProxy_Impl::getFirstChild(void)
{
	materializeChildren();
	return Node_Impl::getFirstChild();
}

std::shared_ptr<dom::Node> ElementProxy_Impl::getLastChild(void)
{
	materializeChildren();
	return Node_Impl::getLastChild();
}

std::shared_ptr<dom::Node> ElementProxy_Impl::getPreviousSibling(void)
{
	return Node_Impl::getPreviousSibling();
}

std::shared_ptr<dom::Node> ElementProxy_Impl::getNextSibling(void)
{
	return Node_Impl::getNextSibling();
}

std::shared_ptr<dom::Node> ElementProxy_Impl::insertBefore(std::shared_ptr<dom::Node> newChild,
  std::shared_ptr<dom::Node> refChild)
{
	materializeChildren();
	return Node_Impl::insertBefore(newChild, refChild);
}

std::shared_ptr<dom::Node> ElementProxy_Impl::replaceChild(std::shared_ptr<dom::Node> newChild,
  std::shared_ptr<dom::Node> oldChild)
{
	materializeChildren();
	return Node_Impl::replaceChild(newChild, oldChild);
}

std::shared_ptr<dom::Node> ElementProxy_Impl::removeChild(std::shared_ptr<dom::Node> oldChild)
{
	materializeChildren();
	return Node_Impl::removeChild(oldChild);
}

std::shared_ptr<dom::Node> ElementProxy_Impl::appendChild(std::shared_ptr<dom::Node> newChild)
{
	materializeChildren();
	return Node_Impl::appendChild(newChild);
}

bool ElementProxy_Impl::hasChildNodes(void)
{
	materializeChildren();
	return Node_Impl::hasChildNodes();
}

void ElementProxy_Impl::materializeChildren()
{
	if (childrenLoaded || !hasDeferredChildren || loadingChildren)
		return;

	loadingChildren	= true;
	XMLTokenizer	tokenizer(sourceFilename);

	tokenizer.restoreState(childTokenizerState);

	for (;;)
	{
		std::shared_ptr<XMLTokenizer::XMLToken>	token(tokenizer.getNextToken());

		switch(token->getTokenType())
		{
		case XMLTokenizer::XMLToken::VALUE:
		{
			std::string	trimmed	= trim(token->getToken());

			if (trimmed.size() > 0)
				Node_Impl::appendChild(factory->createTextNode(trimmed));
			break;
		}
		case XMLTokenizer::XMLToken::TAG_START:
		{
			std::shared_ptr<XMLTokenizer::XMLToken>	tagToken(tokenizer.getNextToken());

			if (tagToken->getTokenType() == XMLTokenizer::XMLToken::ELEMENT)
				Node_Impl::appendChild(createProxyElement(factory, sourceFilename, tokenizer, tagToken->getToken()));
			break;
		}
		case XMLTokenizer::XMLToken::TAG_CLOSE_START:
		{
			std::shared_ptr<XMLTokenizer::XMLToken>	closeTag(tokenizer.getNextToken());
			std::shared_ptr<XMLTokenizer::XMLToken>	tagEnd(tokenizer.getNextToken());
			(void)closeTag;
			(void)tagEnd;
			hasDeferredChildren	= false;
			childrenLoaded		= true;
			loadingChildren		= false;
			return;
		}
		case XMLTokenizer::XMLToken::NULL_TOKEN:
			hasDeferredChildren	= false;
			childrenLoaded		= true;
			loadingChildren		= false;
			return;
		default:
			break;
		}
	}
}

void ElementProxy_Impl::skipElementBody(XMLTokenizer & tokenizer)
{
	int	depth	= 1;

	while (depth > 0)
	{
		std::shared_ptr<XMLTokenizer::XMLToken>	token(tokenizer.getNextToken());

		switch(token->getTokenType())
		{
		case XMLTokenizer::XMLToken::TAG_START:
		{
			std::shared_ptr<XMLTokenizer::XMLToken>	tagName(tokenizer.getNextToken());
			bool					selfClosing	= false;

			(void)tagName;

			for (;;)
			{
				std::shared_ptr<XMLTokenizer::XMLToken>	endTagToken(tokenizer.getNextToken());

				if (endTagToken->getTokenType() == XMLTokenizer::XMLToken::NULL_TAG_END)
				{
					selfClosing	= true;
					break;
				}

				if (endTagToken->getTokenType() == XMLTokenizer::XMLToken::TAG_END ||
				  endTagToken->getTokenType() == XMLTokenizer::XMLToken::NULL_TOKEN)
					break;
			}

			if (!selfClosing)
				depth++;
			break;
		}
		case XMLTokenizer::XMLToken::TAG_CLOSE_START:
		{
			std::shared_ptr<XMLTokenizer::XMLToken>	tagName(tokenizer.getNextToken());
			std::shared_ptr<XMLTokenizer::XMLToken>	tagEnd(tokenizer.getNextToken());
			(void)tagName;
			(void)tagEnd;
			depth--;
			break;
		}
		case XMLTokenizer::XMLToken::NULL_TOKEN:
			return;
		default:
			break;
		}
	}
}

std::string ElementProxy_Impl::getAttributeName(const std::string & token)
{
	std::string	trimmed	= trim(token);
	size_t		equalsAt	= trimmed.find('=');

	if (equalsAt != std::string::npos)
		trimmed	= std::string(trimmed, 0, equalsAt);

	return trim(trimmed);
}

std::string ElementProxy_Impl::getAttributeValue(const std::string & token)
{
	std::string	trimmed	= trim(token);

	if (trimmed.size() >= 2 && trimmed[0] == '"' && trimmed[trimmed.size() - 1] == '"')
		return std::string(trimmed, 1, trimmed.size() - 2);

	return trimmed;
}

std::string ElementProxy_Impl::trim(const std::string & s)
{
	size_t	start_index	= 0;
	size_t	stop_index	= s.size();

	while (start_index < s.size() && std::isspace(static_cast<unsigned char>(s[start_index])))
		start_index++;

	while (stop_index > start_index && std::isspace(static_cast<unsigned char>(s[stop_index - 1])))
		stop_index--;

	if (start_index >= stop_index)
		return std::string("");

	return std::string(s, start_index, stop_index - start_index);
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
