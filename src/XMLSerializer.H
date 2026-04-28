#include <iostream>
#include <memory>

#include "Node.H"

//
// XMLSerializer is Serialization Visitor Context
// java.io.Writer is output stream Strategy
// java.io.BufferedWriter is output stream Concrete Strategy
//
class XMLSerializer
{
private:
	std::ostream *		file;
public:
	XMLSerializer(std::ostream * _file) : file(_file)	{}
	~XMLSerializer() {}
public:
	//
	// Visitor-based serialization
	//
	virtual void		serializePretty(std::shared_ptr<dom::Node> node);
	virtual void		serializeMinimal(std::shared_ptr<dom::Node> node);
};
