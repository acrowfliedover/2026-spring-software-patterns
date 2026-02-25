#include "XMLSerializer.H"

#include "Document.H"
#include "Element.H"
#include "Attr.H"
#include "Text.H"

void XMLSerializer::serializePretty(std::shared_ptr<dom::Node> node)
{
	std::shared_ptr<WhitespaceStrategy>	ws(new PrettyWhitespaceStrategy);
	node->serialize(file, ws);
}

void XMLSerializer::serializeMinimal(std::shared_ptr<dom::Node> node)
{
	std::shared_ptr<WhitespaceStrategy>	ws(new MinimalWhitespaceStrategy);
	node->serialize(file, ws);
}
