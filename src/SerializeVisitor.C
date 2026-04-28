#include "SerializeVisitor.H"

#include "Attr.H"
#include "Document.H"
#include "Element.H"
#include "Text.H"

void SerializeVisitor::visit(dom::Document * document)
{
	*writer << "<? xml version=\"1.0\" encoding=\"UTF-8\"?>";
	whitespace->newLine(writer);
	document->getDocumentElement()->accept(*this);
}

void SerializeVisitor::visit(dom::Element * element)
{
	whitespace->prettyIndentation(writer);
	*writer << "<" << element->getTagName();

	int	attrCount	= 0;

	for (dom::NamedNodeMap::iterator i = element->getAttributes()->begin(); i != element->getAttributes()->end(); i++)
	{
		(*i)->accept(*this);
		attrCount++;
	}

	if (attrCount > 0)
		*writer << " ";

	if (element->getChildNodes()->size() == 0)
	{
		*writer << "/>";
		whitespace->newLine(writer);
	}
	else
	{
		*writer << ">";
		whitespace->newLine(writer);
		whitespace->incrementIndentation();

		for (dom::NodeList::iterator i = element->getChildNodes()->begin(); i != element->getChildNodes()->end(); i++)
			if (std::dynamic_pointer_cast<dom::Element>(*i)  || std::dynamic_pointer_cast<dom::Text>(*i))
				(*i)->accept(*this);

		whitespace->decrementIndentation();
		whitespace->prettyIndentation(writer);
		*writer << "</" << element->getTagName() + ">";
		whitespace->newLine(writer);
	}
}

void SerializeVisitor::visit(dom::Attr * attr)
{
	*writer << " " << attr->getName() << "=\"" << attr->getValue() << "\"";
}

void SerializeVisitor::visit(dom::Text * text)
{
	whitespace->prettyIndentation(writer);
	*writer << text->getData();
	whitespace->newLine(writer);
}
