#include <string>
#include <fstream>
#include <ios>

#include "Node.H"

class XMLSerializer
{
private:
	std::fstream		file;
	int			indentationLevel;

	virtual void		prettyIndentation();
public:
	XMLSerializer(const std::string & filename) : indentationLevel(0), file(filename.c_str(), std::ios_base::out)	{}
	~XMLSerializer() {}

	virtual void		serializePretty(dom::Node * node);
	virtual void		serializeMinimal(dom::Node * node);
};
