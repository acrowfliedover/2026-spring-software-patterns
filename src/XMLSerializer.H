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

public:
	XMLSerializer(const std::string &filename) : file(filename.c_str(), std::ios_base::out) {}
	virtual ~XMLSerializer() {}

	virtual void serialize(dom::Node *node);

	virtual void case_document(dom::Document *node) = 0;
	virtual void case_element(dom::Element *node) = 0;
	virtual void case_text(dom::Text *node) = 0;
};

class PrettyXMLSerializer : public XMLSerializer
{
protected:
	int indentationLevel;
	void prettyIndentation();

public:
	PrettyXMLSerializer(const std::string &filename) : XMLSerializer(filename), indentationLevel(0) {}
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