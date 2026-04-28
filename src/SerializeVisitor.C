#include "SerializeVisitor.H"

#include "Attr.H"
#include "Document.H"
#include "Element.H"
#include "Text.H"

void PrettySerializeVisitor::prettyIndentation(void)
{
	for (int i = 0; i < indentationLevel; i++)
		*writer << "\t";
}

void SerializeVisitor::visit(dom::Document * document)
{
	*writer << "<? xml version=\"1.0\" encoding=\"UTF-8\"?>";
	newLine();
	document->getDocumentElement()->accept(*this);
}

void SerializeVisitor::visit(dom::Element * element)
{
	prettyIndentation();
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
		newLine();
	}
	else
	{
		*writer << ">";
		newLine();
		incrementIndentation();

		for (dom::NodeList::iterator i = element->getChildNodes()->begin(); i != element->getChildNodes()->end(); i++)
			if (std::dynamic_pointer_cast<dom::Element>(*i)  || std::dynamic_pointer_cast<dom::Text>(*i))
				(*i)->accept(*this);

		decrementIndentation();
		prettyIndentation();
		*writer << "</" << element->getTagName() + ">";
		newLine();
	}
}

void SerializeVisitor::visit(dom::Attr * attr)
{
	*writer << " " << attr->getName() << "=\"" << attr->getValue() << "\"";
}

void SerializeVisitor::visit(dom::Text * text)
{
	prettyIndentation();
	*writer << text->getData();
	newLine();
}
