#include "XMLSerializer.H"

#include "SerializeVisitor.H"

void XMLSerializer::serializePretty(std::shared_ptr<dom::Node> node)
{
	std::shared_ptr<WhitespaceStrategy>	ws(new PrettyWhitespaceStrategy);
	SerializeVisitor			visitor(file, ws);
	node->accept(visitor);
}

void XMLSerializer::serializeMinimal(std::shared_ptr<dom::Node> node)
{
	std::shared_ptr<WhitespaceStrategy>	ws(new MinimalWhitespaceStrategy);
	SerializeVisitor			visitor(file, ws);
	node->accept(visitor);
}
