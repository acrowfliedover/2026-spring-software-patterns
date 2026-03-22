#include "Adapter.H"
#include "Element.H"
#include "Text.H"
#include "Attr.H"

const XERCES::XMLCh * adapter::Node::getNodeName() const
{
	return (XERCES::XMLCh *)adaptee->getNodeName().c_str();
}

const XERCES::XMLCh * adapter::Node::getNodeValue() const
{
	return (XERCES::XMLCh *)adaptee->getNodeValue().c_str();
}

XERCES::DOMNode::NodeType adapter::Node::getNodeType() const
{
	switch(adaptee->getNodeType())
	{
	case dom::Node::ATTRIBUTE_NODE:
		return XERCES::DOMNode::ATTRIBUTE_NODE;
	case dom::Node::DOCUMENT_NODE:
		return XERCES::DOMNode::DOCUMENT_NODE;
	case dom::Node::ELEMENT_NODE:
		return XERCES::DOMNode::ELEMENT_NODE;
	case dom::Node::TEXT_NODE:
		return XERCES::DOMNode::TEXT_NODE;
	}

	return XERCES::DOMNode::ELEMENT_NODE;	// Shouldn't get here.
}

XERCES::DOMNode * adapter::Node::getParentNode() const
{
	return dynamic_cast<XERCES::DOMNode *>(new Node(adaptee->getParentNode()));
}

XERCES::DOMNodeList * adapter::Node::getChildNodes() const
{
	return 0; // dynamic_cast<XERCES::DOMNodeList *>(new NodeList(adaptee->getChildNodes()));
}

XERCES::DOMNode * adapter::Node::getFirstChild() const
{
	return dynamic_cast<XERCES::DOMNode *>(new Node(adaptee->getFirstChild().get()));
}

XERCES::DOMNode * adapter::Node::getLastChild() const
{
	return dynamic_cast<XERCES::DOMNode *>(new Node(adaptee->getLastChild().get()));
}

XERCES::DOMNode * adapter::Node::getPreviousSibling() const
{
	return dynamic_cast<XERCES::DOMNode *>(new Node(adaptee->getPreviousSibling().get()));
}

XERCES::DOMNode * adapter::Node::getNextSibling() const
{
	return dynamic_cast<XERCES::DOMNode *>(new Node(adaptee->getNextSibling().get()));
}

XERCES::DOMDocument * adapter::Node::getOwnerDocument() const
{
	return dynamic_cast<XERCES::DOMDocument *>(new Document(adaptee->getOwnerDocument()));
}

XERCES::DOMNode * adapter::Node::insertBefore(XERCES::DOMNode * newChild, XERCES::DOMNode * refChild)
{
	return dynamic_cast<XERCES::DOMNode *>
	  (new Node(adaptee->insertBefore(dynamic_cast<Node *>(newChild)->getAdaptee(), dynamic_cast<Node *>(refChild)->getAdaptee()).get()));
}

XERCES::DOMNode * adapter::Node::replaceChild(XERCES::DOMNode * newChild, XERCES::DOMNode * oldChild)
{
	return dynamic_cast<XERCES::DOMNode *>
	 (new Node(adaptee->replaceChild(dynamic_cast<Node *>(newChild)->getAdaptee(), dynamic_cast<Node *>(oldChild)->getAdaptee()).get()));
}

XERCES::DOMNode * adapter::Node::removeChild(XERCES::DOMNode * oldChild)
{
	return dynamic_cast<XERCES::DOMNode *>(new Node(adaptee->removeChild(dynamic_cast<Node *>(oldChild)->getAdaptee()).get()));
}

XERCES::DOMNode * adapter::Node::appendChild(XERCES::DOMNode * newChild)
{
	return dynamic_cast<XERCES::DOMNode *>(new Node(adaptee->appendChild(dynamic_cast<Node *>(newChild)->getAdaptee()).get()));
}

bool adapter::Node::hasChildNodes() const
{
	return adaptee->hasChildNodes();
}

void adapter::Node::setNodeValue(const XERCES::XMLCh * nodeValue)
{
	adaptee->setNodeValue((char *)nodeValue);
}

XERCES::DOMElement * adapter::Document::createElement(const XERCES::XMLCh * tagName)
{
//	return new Element_Impl(dynamic_cast<dom::Document *>(getAdaptee())->createElement(tagName));
}

XERCES::DOMText * adapter::Document::createTextNode(const XERCES::XMLCh * data)
{
//	return new Text_Impl(dynamic_cast<dom::Document *>(getAdaptee())->createTextNode(data));
}

XERCES::DOMAttr * adapter::Document::createAttribute(const XERCES::XMLCh * name)
{
//	return new Attr_Impl(dynamic_cast<dom::Document *>(getAdaptee())->createAttribute(name));
}

XERCES::DOMElement * adapter::Document::getDocumentElement() const
{
//	return new Element_Impl(dynamic_cast<dom::Document *>(getAdaptee())->getDocumentElement());
}

XERCES::DOMNode * adapter::Document::getParentNode() const
{
	return dynamic_cast<XERCES::DOMDocument *>
	  (new Document(dynamic_cast<dom::Document *>(const_cast<Document *>(this)->getAdaptee()->getParentNode())));
}

XERCES::DOMNode * adapter::Document::getFirstChild() const
{
	return dynamic_cast<XERCES::DOMDocument *>
	  (new Document(dynamic_cast<dom::Document *>(const_cast<Document *>(this)->getAdaptee()->getFirstChild())));
}

XERCES::DOMNode * adapter::Document::getLastChild() const
{
	return dynamic_cast<XERCES::DOMDocument *>
	  (new Document(dynamic_cast<dom::Document *>(const_cast<Document *>(this)->getAdaptee()->getLastChild())));
}

XERCES::DOMNode * adapter::Document::getPreviousSibling() const
{
	return dynamic_cast<XERCES::DOMDocument *>
	  (new Document(dynamic_cast<dom::Document *>(const_cast<Document *>(this)->getAdaptee()->getPreviousSibling())));
}

XERCES::DOMNode * adapter::Document::getNextSibling() const
{
	return dynamic_cast<XERCES::DOMDocument *>
	  (new Document(dynamic_cast<dom::Document *>(const_cast<Document *>(this)->getAdaptee()->getNextSibling())));
}

XERCES::DOMNode * adapter::Document::insertBefore(XERCES::DOMNode * newChild, XERCES::DOMNode * refChild)
{
	return 0;
}

XERCES::DOMNode * adapter::Document::replaceChild(XERCES::DOMNode * newChild, XERCES::DOMNode * oldChild)
{
	return 0;
}

XERCES::DOMNode * adapter::Document::removeChild(XERCES::DOMNode * oldChild)
{
	return 0;
}

XERCES::DOMNode * adapter::Document::appendChild(XERCES::DOMNode * newChild)
{
	return 0;
}
