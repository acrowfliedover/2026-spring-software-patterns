#include "Node.H"

Node_Impl::Node_Impl(const std::string & n, short type) : name(n), nodeType(type), parent(0), document(0)
{
}

Node_Impl::~Node_Impl()
{
}

const std::string &	Node_Impl::getNodeName(void)
{
	return name;
}

const std::string &	Node_Impl::getNodeValue(void)
{
	return value;
}

void			Node_Impl::setNodeValue(const std::string & nodeValue)
{
	value	= nodeValue;
}

short			Node_Impl::getNodeType(void)
{
	return nodeType;
}

dom::Node *		Node_Impl::getParentNode(void)
{
	return parent;
}

dom::NodeList *		Node_Impl::getChildNodes(void)
{
	return &nodes;
}

std::shared_ptr<dom::Node>		Node_Impl::getFirstChild(void)
{
	return *nodes.begin();
}

std::shared_ptr<dom::Node>		Node_Impl::getLastChild(void)
{
	return *(--nodes.end());
}

std::shared_ptr<dom::Node>		Node_Impl::getPreviousSibling(void)
{
	return getSibling(-1);
}

std::shared_ptr<dom::Node>		Node_Impl::getNextSibling(void)
{
	return getSibling(1);
}

dom::Document *		Node_Impl::getOwnerDocument(void)
{
	return document;
}

std::shared_ptr<dom::Node>		Node_Impl::insertBefore(std::shared_ptr<dom::Node> newChild,
					  std::shared_ptr<dom::Node> refChild)
{
	if (newChild->getOwnerDocument() != getOwnerDocument())
		throw dom::DOMException(dom::DOMException::WRONG_DOCUMENT_ERR, "New Child is not a part of this document.");

	if (newChild->getParentNode() != 0)
		newChild->getParentNode()->removeChild(newChild);

	if (refChild == 0)
	{
		nodes.push_back(newChild);
		(std::dynamic_pointer_cast<Node_Impl>(newChild))->setParent(this);
		return newChild;
	}

	dom::NodeList::const_iterator	index	= nodes.find(refChild.get());

	if (index == nodes.end())
		throw dom::DOMException(dom::DOMException::NOT_FOUND_ERR, "Reference Child is not a child of this node.");

	nodes.insert(++index, newChild);
	(std::dynamic_pointer_cast<Node_Impl>(newChild))->setParent(this);

	return newChild;
}

std::shared_ptr<dom::Node>		Node_Impl::replaceChild(std::shared_ptr<dom::Node> newChild,
					  std::shared_ptr<dom::Node> oldChild)
{
	if (newChild->getOwnerDocument() != getOwnerDocument())
		throw dom::DOMException(dom::DOMException::WRONG_DOCUMENT_ERR, "New Child is not a part of this document.");

	if (newChild->getParentNode() != 0)
		newChild->getParentNode()->removeChild(newChild);

	dom::NodeList::const_iterator	index	= nodes.find(oldChild.get());

	if (index == nodes.end())
		throw dom::DOMException(dom::DOMException::NOT_FOUND_ERR, "Old Child is not a child of this node.");

	nodes.insert(index, newChild);
	(std::dynamic_pointer_cast<Node_Impl>(newChild))->setParent(this);
	(std::dynamic_pointer_cast<Node_Impl>(*index))->setParent(0);
	nodes.erase(index);

	return oldChild;
}

std::shared_ptr<dom::Node>		Node_Impl::removeChild(std::shared_ptr<dom::Node> oldChild)
{
	dom::NodeList::const_iterator	index	= nodes.find(oldChild.get());

	if (index == nodes.end())
		throw dom::DOMException(dom::DOMException::NOT_FOUND_ERR, "Old Child is not a child of this node.");

	(std::dynamic_pointer_cast<Node_Impl>(*index))->setParent(0);
	nodes.erase(index);

	return oldChild;
}

std::shared_ptr<dom::Node>		Node_Impl::appendChild(std::shared_ptr<dom::Node> newChild)
{
	if (newChild->getOwnerDocument() != getOwnerDocument())
		throw dom::DOMException(dom::DOMException::WRONG_DOCUMENT_ERR, "New Child is not a part of this document.");

	if (newChild->getParentNode() != 0)
		newChild->getParentNode()->removeChild(newChild);

	nodes.push_back(newChild);
	(std::dynamic_pointer_cast<Node_Impl>(newChild))->setParent(this);

	return newChild;
}

bool			Node_Impl::hasChildNodes(void)
{
	return nodes.size() > 0;
}

const std::string &	Node_Impl::getLocalName(void)
{
	return name;
}

void Node_Impl::setParent(dom::Node * parent)
{
	this->parent	= parent;
}

std::shared_ptr<dom::Node>		Node_Impl::getSibling(int direction)
{
	if (parent == 0)
		return 0;

	dom::NodeList::iterator	i	= parent->getChildNodes()->find(this);

	if (direction < 0)
	{
		if (i == parent->getChildNodes()->begin())
			return 0;
		else
			return *(--i);
	}
	else
	{
		i++;

		if (i == parent->getChildNodes()->end())
			return 0;
		else
			return *i;
	}
}

std::shared_ptr<dom::Node> Node_Impl::cloneNode(bool deep)
{
	return std::shared_ptr<Node_Impl>();
}
