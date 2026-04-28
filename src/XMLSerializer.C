#include "XMLSerializer.H"

#include "SerializeVisitor.H"

void XMLSerializer::serializePretty(std::shared_ptr<dom::Node> node)
{
	PrettySerializeVisitor	visitor(file);
	node->accept(visitor);
}

void XMLSerializer::serializeMinimal(std::shared_ptr<dom::Node> node)
{
	MinimalSerializeVisitor	visitor(file);
	node->accept(visitor);
}
