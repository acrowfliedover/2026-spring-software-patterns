#include "XMLSerializer.H"

#include "Document.H"
#include "Element.H"
#include "Attr.H"
#include "Text.H"

void XMLSerializer::prettyIndentation()
{
	for (int i = 0; i < indentationLevel; i++)
		file << "\t";
}

void XMLSerializer::serializePretty(dom::Node * node)
{
	if (dynamic_cast<dom::Document *>(node) != 0)
	{
		file << "<? xml version=\"1.0\" encoding=\"UTF-8\"?>";
		file << "\n";
		serializePretty(dynamic_cast<dom::Document *>(node)->getDocumentElement());
	}
	else if (dynamic_cast<dom::Element *>(node) != 0)
	{
		prettyIndentation();
		file << "<" << dynamic_cast<dom::Element *>(node)->getTagName();

		int	attrCount	= 0;

		for (dom::NamedNodeMap::iterator i = dynamic_cast<dom::Element *>(node)->getAttributes()->begin();
		  i != dynamic_cast<dom::Element *>(node)->getAttributes()->end();
		  i++)
		{
			serializePretty(*i);
			attrCount++;
		}

		if (attrCount > 0)
			file << " ";

		if (dynamic_cast<dom::Element *>(node)->getChildNodes()->size() == 0)
		{
			file << "/>";
			file << "\n";
		}
		else
		{
			file << ">";
			file << "\n";
			indentationLevel++;

			for (dom::NodeList::iterator i = dynamic_cast<dom::Element *>(node)->getChildNodes()->begin();
			  i != dynamic_cast<dom::Element *>(node)->getChildNodes()->end();
			  i++)
				if (dynamic_cast<dom::Element *>(*i) != 0 || dynamic_cast<dom::Text *>(*i) != 0)
					serializePretty(*i);

			indentationLevel--;
			prettyIndentation();
			file << "</" << dynamic_cast<dom::Element *>(node)->getTagName() + ">";
			file << "\n";
		}
	}
	else if (dynamic_cast<dom::Attr *>(node) != 0)
	{
		file <<
		  " " << dynamic_cast<dom::Attr *>(node)->getName() << "=\"" << dynamic_cast<dom::Attr *>(node)->getValue() << "\"";
	}
	else if (dynamic_cast<dom::Text *>(node) != 0)
	{
		prettyIndentation();
		file << dynamic_cast<dom::Text *>(node)->getData();
		file << "\n";
	}
}

void XMLSerializer::serializeMinimal(dom::Node * node)
{
	if (dynamic_cast<dom::Document *>(node) != 0)
	{
		file << "<? xml version=\"1.0\" encoding=\"UTF-8\"?>";
		serializeMinimal(dynamic_cast<dom::Document *>(node)->getDocumentElement());
	}
	else if (dynamic_cast<dom::Element *>(node) != 0)
	{
		file << "<" << dynamic_cast<dom::Element *>(node)->getTagName();

		for (dom::NamedNodeMap::iterator i = dynamic_cast<dom::Element *>(node)->getAttributes()->begin();
		  i != dynamic_cast<dom::Element *>(node)->getAttributes()->end();
		  i++)
			serializeMinimal(*i);

		if (dynamic_cast<dom::Element *>(node)->getChildNodes()->size() == 0)
			file << "/>";
		else
		{
			file << ">";

			for (dom::NodeList::iterator i = dynamic_cast<dom::Element *>(node)->getChildNodes()->begin();
			  i != dynamic_cast<dom::Element *>(node)->getChildNodes()->end();
			  i++)
				if (dynamic_cast<dom::Element *>(*i) != 0 || dynamic_cast<dom::Text *>(*i) != 0)
					serializeMinimal(*i);

			file << "</" << dynamic_cast<dom::Element *>(node)->getTagName() + ">";
		}
	}
	else if (dynamic_cast<dom::Attr *>(node) != 0)
	{
		file <<
		  " " << dynamic_cast<dom::Attr *>(node)->getName() << "=\"" << dynamic_cast<dom::Attr *>(node)->getValue() << "\"";
	}
	else if (dynamic_cast<dom::Text *>(node) != 0)
		file << dynamic_cast<dom::Text *>(node)->getData();
}
