#include "XMLSerializer.H"
#include "Document.H"
#include "Element.H"
#include "Attr.H"
#include "Text.H"

XMLSerializer::XMLSerializer(const std::string & filename)
	: file(filename.c_str(), std::ios_base::out), indentationLevel(0), pretty(false) {}

void XMLSerializer::prettyIndentation()
{
	for (int i = 0; i < indentationLevel; i++)
		file << "\t";
}

void XMLSerializer::serializePretty(dom::Node * node) //context
{
	pretty = true;
	indentationLevel = 0;
	node->serializeWith(*this);
}

void XMLSerializer::serializeMinimal(dom::Node * node) //context
{
	pretty = false;
	node->serializeWith(*this);
}

void XMLSerializer::serialize(dom::Document * doc) //concrete strategy
{
	file << "<? xml version=\"1.0\" encoding=\"UTF-8\"?>";
	if (pretty) file << "\n";
	doc->getDocumentElement()->serializeWith(*this);
}

void XMLSerializer::serialize(dom::Element * el) //concrete strategy
{
	if (pretty) prettyIndentation();
	file << "<" << el->getTagName();
	for (dom::NamedNodeMap::iterator i = el->getAttributes()->begin(); i != el->getAttributes()->end(); ++i)
		(*i)->serializeWith(*this);
	if (el->getAttributes()->size() > 0) file << " ";
	if (el->getChildNodes()->size() == 0)
	{
		file << "/>";
		if (pretty) file << "\n";
	}
	else
	{
		file << ">";
		if (pretty) { file << "\n"; indentationLevel++; }
		for (dom::NodeList::iterator i = el->getChildNodes()->begin(); i != el->getChildNodes()->end(); ++i)
			if ((*i)->getNodeType() == 2 || (*i)->getNodeType() == 3)
				(*i)->serializeWith(*this);
		if (pretty) indentationLevel--;
		if (pretty) prettyIndentation();
		file << "</" << el->getTagName() << ">";
		if (pretty) file << "\n";
	}
}

void XMLSerializer::serialize(dom::Attr * a) //concrete strategy
{
	file << " " << a->getName() << "=\"" << a->getValue() << "\"";
}

void XMLSerializer::serialize(dom::Text * t) //concrete strategy
{
	if (pretty) prettyIndentation();
	file << t->getData();
	if (pretty) file << "\n";
}
