#include "NodeFlyweight.H"
#include "Node.H"

#include <stdexcept>

dom::ElementFlyweight::ElementFlyweight(const std::string & n) : tagName(n)
{
}

short dom::ElementFlyweight::getNodeType(void) const
{
	return dom::Node::ELEMENT_NODE;
}

const std::string & dom::ElementFlyweight::getName(void) const
{
	return tagName;
}

dom::AttrFlyweight::AttrFlyweight(const std::string & n) : attrName(n)
{
}

short dom::AttrFlyweight::getNodeType(void) const
{
	return dom::Node::ATTRIBUTE_NODE;
}

const std::string & dom::AttrFlyweight::getName(void) const
{
	return attrName;
}

const std::string dom::TextFlyweight::emptyName("");

dom::TextFlyweight::TextFlyweight(void)
{
}

short dom::TextFlyweight::getNodeType(void) const
{
	return dom::Node::TEXT_NODE;
}

const std::string & dom::TextFlyweight::getName(void) const
{
	return emptyName;
}

const std::string dom::DocumentFlyweight::emptyName("");

dom::DocumentFlyweight::DocumentFlyweight(void)
{
}

short dom::DocumentFlyweight::getNodeType(void) const
{
	return dom::Node::DOCUMENT_NODE;
}

const std::string & dom::DocumentFlyweight::getName(void) const
{
	return emptyName;
}

dom::NodeFlyweightFactory & dom::NodeFlyweightFactory::instance(void)
{
	static NodeFlyweightFactory	singleton;

	return singleton;
}

std::shared_ptr<const dom::NodeFlyweight> dom::NodeFlyweightFactory::createFlyweight(short nodeType, const std::string & name)
{
	switch (nodeType)
	{
	case dom::Node::ATTRIBUTE_NODE:
		return std::shared_ptr<const NodeFlyweight>(new AttrFlyweight(name));
	case dom::Node::DOCUMENT_NODE:
		return std::shared_ptr<const NodeFlyweight>(new DocumentFlyweight());
	case dom::Node::ELEMENT_NODE:
		return std::shared_ptr<const NodeFlyweight>(new ElementFlyweight(name));
	case dom::Node::TEXT_NODE:
		return std::shared_ptr<const NodeFlyweight>(new TextFlyweight());
	default:
		throw std::invalid_argument("NodeFlyweightFactory::createFlyweight: unknown node type");
	}
}

std::shared_ptr<const dom::NodeFlyweight> dom::NodeFlyweightFactory::getFlyweight(short nodeType, const std::string & name)
{
	Key	key(nodeType, name);

	std::map<Key, std::shared_ptr<const NodeFlyweight>>::iterator	i	= pool.find(key);

	if (i != pool.end())
		return i->second;

	std::shared_ptr<const NodeFlyweight>	flyweight	= createFlyweight(nodeType, name);

	pool[key]	= flyweight;

	return flyweight;
}
