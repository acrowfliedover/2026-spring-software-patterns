#include "XMLSerializer.H"

#include "Document.H"
#include "Element.H"
#include "Attr.H"
#include "Text.H"

void XMLSerializer::serialize(dom::Node *node){
	if (dynamic_cast<dom::Document *>(node) != 0) {
		case_document(dynamic_cast<dom::Document *>(node));
	} else if (dynamic_cast<dom::Element *>(node) != 0) {
		case_element(dynamic_cast<dom::Element *>(node));
	} else if (dynamic_cast<dom::Attr *>(node) != 0) {
		file << " " << dynamic_cast<dom::Attr *>(node)->getName() << "=\"" << dynamic_cast<dom::Attr *>(node)->getValue() << "\"";
	} else if (dynamic_cast<dom::Text *>(node) != 0) {
		case_text(dynamic_cast<dom::Text *>(node));
	}
}

void PrettyXMLSerializer::prettyIndentation()
{
	for (int i = 0; i < indentationLevel; i++)
		file << "\t";
}

void PrettyXMLSerializer::case_document(dom::Document *node)
{
	file << "<? xml version=\"1.0\" encoding=\"UTF-8\"?>";
	file << "\n";
	serialize(node->getDocumentElement());
}

void PrettyXMLSerializer::case_element(dom::Element *node)
{
	prettyIndentation();
	file << "<" << node->getTagName();

	int attrCount = 0;

	for (dom::NamedNodeMap::iterator i = node->getAttributes()->begin();
		 i != node->getAttributes()->end();
		 i++)
	{
		serialize(*i);
		attrCount++;
	}

	if (attrCount > 0)
		file << " ";

	if (node->getChildNodes()->size() == 0)
	{
		file << "/>";
		file << "\n";
	}
	else
	{
		file << ">";
		file << "\n";
		indentationLevel++;

		for (dom::NodeList::iterator i = node->getChildNodes()->begin();
			 i != node->getChildNodes()->end();
			 i++)
			if (dynamic_cast<dom::Element *>(*i) != 0 || dynamic_cast<dom::Text *>(*i) != 0)
				serialize(*i);

		indentationLevel--;
		prettyIndentation();
		file << "</" << node->getTagName() + ">";
		file << "\n";
	}
}

void PrettyXMLSerializer::case_text(dom::Text *node)
{
	prettyIndentation();
	file << node->getData();
	file << "\n";
}

void MinimalXMLSerializer::case_document(dom::Document *node)
{
	file << "<? xml version=\"1.0\" encoding=\"UTF-8\"?>";
	serialize(node->getDocumentElement());
}

void MinimalXMLSerializer::case_element(dom::Element *node)
{
	file << "<" << node->getTagName();

	for (dom::NamedNodeMap::iterator i = node->getAttributes()->begin();
		 i != node->getAttributes()->end();
		 i++)
		serialize(*i);

	if (node->getChildNodes()->size() == 0)
		file << "/>";
	else
	{
		file << ">";

		for (dom::NodeList::iterator i = node->getChildNodes()->begin();
			 i != node->getChildNodes()->end();
			 i++)
			if (dynamic_cast<dom::Element *>(*i) != 0 || dynamic_cast<dom::Text *>(*i) != 0)
				serialize(*i);

		file << "</" << node->getTagName() + ">";
	}
}

void MinimalXMLSerializer::case_text(dom::Text *node)
{
	file << node->getData();
}
