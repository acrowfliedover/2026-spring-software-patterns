#include <string>
#include <fstream>
#include <ios>

#include "Node.H"

namespace dom { class Document; class Element; class Attr; class Text; }

class XMLSerializer
{
private:
	std::fstream		file;
	int			indentationLevel;
	bool			pretty;
	
	virtual void prettyIndentation();
public:
	XMLSerializer(const std::string & filename);

	virtual void serializePretty(dom::Node * node);
	virtual void serializeMinimal(dom::Node * node);

	void serialize(dom::Document * node);
	void serialize(dom::Element * node);
	void serialize(dom::Attr * node);
	void serialize(dom::Text * node);

};
