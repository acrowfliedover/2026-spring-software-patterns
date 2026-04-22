#include "Document.H"
#include "Element.H"
#include "Text.H"
#include "Attr.H"
#include "NodeList.H"
#include "XMLValidator.H"

Document_Impl::Document_Impl(void) : Node_Impl("", dom::Node::DOCUMENT_NODE)
{
	Node_Impl::document	= this;
}

Document_Impl::~Document_Impl() {}

void Document_Impl::serialize(std::ostream * writer, std::shared_ptr<WhitespaceStrategy> whitespace)
{
	*writer << "<? xml version=\"1.0\" encoding=\"UTF-8\"?>";
	whitespace->newLine(writer);
	getDocumentElement()->serialize(writer, whitespace);
}

std::shared_ptr<dom::Element>	Document_Impl::createElement(const std::string & tagName)
{
	return std::shared_ptr<Element_Impl>(new Element_Impl(tagName, this));
}

std::shared_ptr<dom::Text>	Document_Impl::createTextNode(const std::string & data)
{
	return std::shared_ptr<Text_Impl>(new Text_Impl(data, this));
}

std::shared_ptr<dom::Attr>	Document_Impl::createAttribute(const std::string & name)
{
	return std::shared_ptr<Attr_Impl>(new Attr_Impl(name, this));
}

std::shared_ptr<dom::Element> Document_Impl::getDocumentElement()
{
	for (dom::NodeList::iterator i = getChildNodes()->begin(); i != getChildNodes()->end(); i++)
		if (std::dynamic_pointer_cast<dom::Element>(*i))
			return std::dynamic_pointer_cast<dom::Element>(*i);

	return 0;
}

std::shared_ptr<dom::Iterator> Document_Impl::createIterator(dom::Node * node)
{
	return std::shared_ptr<DOMIterator>(new DOMIterator(node, this));
}

std::shared_ptr<dom::Node> Document_Impl::cloneNode(bool deep)
{
	return std::shared_ptr<dom::Node>();	// This implementation doesn't have the ability to reparent a cloned tree
						// into a new document.  Therefore it can't usefully support cloning of Document.
}

std::shared_ptr<dom::Node> DocumentValidator::cloneNode(bool deep)
{
	return component->cloneNode(deep);
}

DocumentValidator::DocumentValidator(dom::Document * _component, std::shared_ptr<XMLValidator> xmlValidator) :
  Node_Impl("", dom::Node::DOCUMENT_NODE),
  component(_component),
  schemaElement(*xmlValidator->findSchemaElement("")),
  validator(xmlValidator)
{
}

std::shared_ptr<dom::Element>	DocumentValidator::createElement(const std::string & tagName)
{
	return std::make_shared<ElementValidator>(component->createElement(tagName), validator);
}

std::shared_ptr<dom::Node> DocumentValidator::insertBefore(std::shared_ptr<dom::Node> newChild, std::shared_ptr<dom::Node> refChild)
{
	if (schemaElement == 0 || schemaElement->childIsValid(newChild->getNodeName(), false))
		return component->insertBefore(newChild, refChild);
	else
		throw dom::DOMException(dom::DOMException::VALIDATION_ERR, "Invalid root node " + newChild->getNodeName() + ".");
}

std::shared_ptr<dom::Node> DocumentValidator::replaceChild(std::shared_ptr<dom::Node> newChild, std::shared_ptr<dom::Node> oldChild)
{
	if (schemaElement == 0 || schemaElement->childIsValid(newChild->getNodeName(), false))
		return component->replaceChild(newChild, oldChild);
	else
		throw dom::DOMException(dom::DOMException::VALIDATION_ERR, "Invalid root node " + newChild->getNodeName() + ".");
}

std::shared_ptr<dom::Node> DocumentValidator::appendChild(std::shared_ptr<dom::Node> newChild)
{
	if (schemaElement == 0 || schemaElement->childIsValid(newChild->getNodeName(), false))
		return component->appendChild(newChild);
	else
		throw dom::DOMException(dom::DOMException::VALIDATION_ERR, "Invalid root node " + newChild->getNodeName() + ".");
}

DOMIterator::DOMIterator(dom::Node * startWithNode, Document_Impl * document) :
  firstNode(startWithNode == 0 ? (dom::Node * )document->getDocumentElement().get() : startWithNode)
{
	if (firstNode != 0)
		for (dom::Node * node = firstNode; node->getChildNodes()->size() > 0; node = node->getChildNodes()->begin()->get())
		{
			listStack.push(node->getChildNodes());
			indexStack.push(0);
		}
}

dom::Node * DOMIterator::elementAt(dom::NodeList * currentList, int currentIndex)
{
	int			i;
	dom::NodeList::iterator	it;

	for (i = 0, it = currentList->begin(); it != currentList->end() && i < currentIndex; i++, it++);

	return it->get();
}

bool DOMIterator::hasNext()
{
	return firstNode != 0;
}

dom::Node * DOMIterator::next()
{
	dom::NodeList *	currentList(listStack.size() > 0 ? listStack.top() : 0);

	if (currentList == 0)
	{
		dom::Node *	temp	= firstNode;
		firstNode		= 0;
		return temp;
	}
	else
	{
		int		currentIndex	= indexStack.top();
		indexStack.pop();
		dom::Node *	temp		= elementAt(currentList, currentIndex++);

		if (currentIndex >= currentList->size())
			listStack.pop();
		else
		{
			indexStack.push(currentIndex);

			for (dom::Node * node = elementAt(currentList, currentIndex);
			  node->getChildNodes()->size() > 0;
			  node = node->getChildNodes()->begin()->get())
			{
				listStack.push(node->getChildNodes());
				indexStack.push(0);
			}
		}

		return temp;
	}
}
