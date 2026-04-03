#include <stdio.h>
#include "StdOutObserver.H"
#include "Node.H"


void StdOutObserver::update(std::shared_ptr<dom::Node> container, short targetType, const std::string & target)
{
	switch(targetType)
	{
	case dom::Node::ELEMENT_NODE:
		printf("Validating Element %s in parent %s\n",
		  target.c_str(), container ? container->getNodeName().c_str() : "<none>");
		break;
	case dom::Node::ATTRIBUTE_NODE:
		printf("Validating Attribute %s in parent %s\n",
		  target.c_str(), container ? container->getNodeName().c_str() : "<none>");
		break;
	case dom::Node::TEXT_NODE:
		printf("Validating Text %s in parent %s\n",
		  target.c_str(), container ? container->getNodeName().c_str() : "<none>");
		break;
	default:
		printf("Validating unknown node type %d\n", targetType);
		break;
	}
}
