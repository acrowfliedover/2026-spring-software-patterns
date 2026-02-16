#include "Document.H"
#include "Element.H"
#include "Text.H"
#include "Attr.H"
#include "NodeList.H"

Document_Impl::Document_Impl(void) : Node_Impl("", dom::Node::DOCUMENT_NODE)
{
	Node_Impl::document = this;
}

Document_Impl::~Document_Impl() {}

void Document_Impl::setSingleChild(dom::Node *child)
{
	if (childList.size() > 0)
	{
		(dynamic_cast<Node_Impl *>(*childList.begin()))->setParent(0);
		childList.clear();
	}
	if (child)
	{
		childList.push_back(child);
		(dynamic_cast<Node_Impl *>(child))->setParent(this);
	}
}

dom::Element *Document_Impl::createElement(const std::string &tagName)
{
	return new Element_Impl(tagName, this);
}

dom::Text *Document_Impl::createTextNode(const std::string &data)
{
	return new Text_Impl(data, this);
}

dom::Attr *Document_Impl::createAttribute(const std::string &name)
{
	return new Attr_Impl(name, this);
}

dom::Element *Document_Impl::getDocumentElement()
{
	dom::Node *child = getFirstChild();
	return child ? dynamic_cast<dom::Element *>(child) : 0;
}

dom::NodeList *Document_Impl::getChildNodes(void)
{
	return &childList;
}

dom::Node *Document_Impl::getFirstChild(void)
{
	return childList.size() == 0 ? 0 : *childList.begin();
}

dom::Node *Document_Impl::getLastChild(void)
{
	return childList.size() == 0 ? 0 : *(--childList.end());
}

dom::Node *Document_Impl::insertBefore(dom::Node *newChild, dom::Node *refChild)
{
	if (dynamic_cast<dom::Element *>(newChild) == 0)
		throw dom::DOMException(dom::DOMException::HIERARCHY_REQUEST_ERR, "Document may only have an Element as its child.");

	if (newChild->getOwnerDocument() != getOwnerDocument())
		throw dom::DOMException(dom::DOMException::WRONG_DOCUMENT_ERR, "New Child is not a part of this document.");

	dom::Node *parent = newChild->getParentNode();
	if (parent != 0)
		parent->removeChild(newChild);

	setSingleChild(newChild);
	return newChild;
}

dom::Node *Document_Impl::replaceChild(dom::Node *newChild, dom::Node *oldChild)
{
	if (dynamic_cast<dom::Element *>(newChild) == 0)
		throw dom::DOMException(dom::DOMException::HIERARCHY_REQUEST_ERR, "Document may only have an Element as its child.");

	if (newChild->getOwnerDocument() != getOwnerDocument())
		throw dom::DOMException(dom::DOMException::WRONG_DOCUMENT_ERR, "New Child is not a part of this document.");

	if (childList.size() == 0 || *childList.begin() != oldChild)
		throw dom::DOMException(dom::DOMException::NOT_FOUND_ERR, "Old Child is not a child of this node.");

	dom::Node *parent = newChild->getParentNode();
	if (parent != 0)
		parent->removeChild(newChild);

	(dynamic_cast<Node_Impl *>(oldChild))->setParent(0);
	setSingleChild(newChild);
	return oldChild;
}

dom::Node *Document_Impl::removeChild(dom::Node *oldChild)
{
	if (childList.size() == 0 || *childList.begin() != oldChild)
		throw dom::DOMException(dom::DOMException::NOT_FOUND_ERR, "Old Child is not a child of this node.");

	(dynamic_cast<Node_Impl *>(*childList.begin()))->setParent(0);
	childList.clear();
	return oldChild;
}

dom::Node *Document_Impl::appendChild(dom::Node *newChild)
{
	if (dynamic_cast<dom::Element *>(newChild) == 0)
		throw dom::DOMException(dom::DOMException::HIERARCHY_REQUEST_ERR, "Document may only have an Element as its child.");

	if (newChild->getOwnerDocument() != getOwnerDocument())
		throw dom::DOMException(dom::DOMException::WRONG_DOCUMENT_ERR, "New Child is not a part of this document.");

	dom::Node *parent = newChild->getParentNode();
	if (parent != 0)
		parent->removeChild(newChild);

	setSingleChild(newChild);
	return newChild;
}

bool Document_Impl::hasChildNodes(void)
{
	return childList.size() > 0;
}
