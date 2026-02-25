#include "NamedNodeMap.H"
#include "Attr.H"
#include "Element.H"

NamedNodeMap_Impl::NamedNodeMap_Impl(dom::Document * _document) : document(_document)
{
}

NamedNodeMap_Impl::~NamedNodeMap_Impl()
{
}

std::shared_ptr<dom::Node>	NamedNodeMap_Impl::getNamedItem(const std::string & name)
{
	for (iterator i = begin(); i != end(); i++)
		if (((*i).operator->())->getNodeName().compare(name) == 0)
			return *i;

	return 0;
}

std::shared_ptr<dom::Node>	NamedNodeMap_Impl::setNamedItem(std::shared_ptr<dom::Node> arg)
{
	if (arg->getOwnerDocument() != document)
		throw dom::DOMException(dom::DOMException::WRONG_DOCUMENT_ERR, "Arg not created by this document.");

	std::shared_ptr<dom::Attr>	attribute;

	if ((attribute = std::dynamic_pointer_cast<dom::Attr>(arg)) != 0)
	{
		dom::Node *	parent(attribute->getParentNode());

		if (parent != 0 && dynamic_cast<dom::Element *>(parent) != 0)
			throw dom::DOMException(dom::DOMException::INUSE_ATTRIBUTE_ERR, "Arg not created by this document.");
	}

	iterator	i;

	for (i = begin(); i != end(); i++)
		if (((*i).operator->())->getNodeName().compare(arg->getNodeName()) == 0)
			break;

	if (i != end())
		erase(i);

	push_back(arg);

	return *i;
}

std::shared_ptr<dom::Node>	NamedNodeMap_Impl::removeNamedItem(const std::string & name)
{
	for (iterator i = begin(); i != end(); i++)
		if (((*i).operator->())->getNodeName().compare(name) == 0)
			return *i;

	throw dom::DOMException(dom::DOMException::NOT_FOUND_ERR, "Node not found.");
}
