#include <string>
#include <fstream>
#include <ios>

#include "Node.H"

namespace dom
{
	class Document;
	class Element;
	class Attr;
	class Text;
}

class XMLSerializer
{
protected:
	std::fstream file;
	int indentationLevel;

	virtual void prettyIndentation();

public:
	XMLSerializer(const std::string &filename) : indentationLevel(0), file(filename.c_str(), std::ios_base::out) {}
	~XMLSerializer() {}

	virtual void serialize(dom::Node *node);

	virtual void case_document(dom::Document *node);
	virtual void case_element(dom::Element *node);
	virtual void case_text(dom::Text *node);
};

class PrettyXMLSerializer : public XMLSerializer
{
public:
	PrettyXMLSerializer(const std::string &filename) : XMLSerializer(filename) {}
	~PrettyXMLSerializer() {}

	virtual void case_document(dom::Document *node);
	virtual void case_element(dom::Element *node);
	virtual void case_text(dom::Text *node);
};

class MinimalXMLSerializer : public XMLSerializer
{
public:
	MinimalXMLSerializer(const std::string &filename) : XMLSerializer(filename) {}
	~MinimalXMLSerializer() {}

	virtual void case_document(dom::Document *node);
	virtual void case_element(dom::Element *node);
	virtual void case_text(dom::Text *node);
};