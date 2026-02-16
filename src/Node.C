#include "Node.H"
#include "NodeList.H"

Node_Impl::Node_Impl(const std::string &n, short type) : name(n), nodeType(type), parent(0), document(0)
{
}

Node_Impl::~Node_Impl()
{
}

const std::string &Node_Impl::getNodeName(void)
{
	return name;
}

const std::string &Node_Impl::getNodeValue(void)
{
	return value;
}

void Node_Impl::setNodeValue(const std::string &nodeValue)
{
	value = nodeValue;
}

short Node_Impl::getNodeType(void)
{
	return nodeType;
}

dom::Node *Node_Impl::getParentNode(void)
{
	return parent;
}

dom::NodeList *Node_Impl::getChildNodes(void)
{
	//
	// Default "Leaf" behavior for Composite:
	// nodes that aren't composites have no children.
	//
	static dom::NodeList empty;
	return &empty;
}

dom::Node *Node_Impl::getFirstChild(void)
{
	return 0;
}

dom::Node *Node_Impl::getLastChild(void)
{
	return 0;
}

dom::Node *Node_Impl::getPreviousSibling(void)
{
	return getSibling(-1);
}

dom::Node *Node_Impl::getNextSibling(void)
{
	return getSibling(1);
}

dom::Document *Node_Impl::getOwnerDocument(void)
{
	return document;
}

dom::Node *Node_Impl::insertBefore(dom::Node *newChild, dom::Node *refChild)
{
	throw dom::DOMException(dom::DOMException::HIERARCHY_REQUEST_ERR, "This node type cannot have children.");
}

dom::Node *Node_Impl::replaceChild(dom::Node *newChild, dom::Node *oldChild)
{
	throw dom::DOMException(dom::DOMException::HIERARCHY_REQUEST_ERR, "This node type cannot have children.");
}

dom::Node *Node_Impl::removeChild(dom::Node *oldChild)
{
	throw dom::DOMException(dom::DOMException::HIERARCHY_REQUEST_ERR, "This node type cannot have children.");
}

dom::Node *Node_Impl::appendChild(dom::Node *newChild)
{
	throw dom::DOMException(dom::DOMException::HIERARCHY_REQUEST_ERR, "This node type cannot have children.");
}

bool Node_Impl::hasChildNodes(void)
{
	return false;
}

const std::string &Node_Impl::getLocalName(void)
{
	return name;
}

void Node_Impl::setParent(dom::Node *parent)
{
	this->parent = parent;
}

dom::Node *Node_Impl::getSibling(int direction)
{
	if (parent == 0)
		return 0;

	dom::NodeList *childList = parent->getChildNodes();
	dom::NodeList::iterator i = childList->find(this);

	if (direction < 0)
	{
		if (i == childList->begin())
			return 0;
		else
			return *(--i);
	}
	else
	{
		i++;

		if (i == childList->end())
			return 0;
		else
			return *i;
	}
}
