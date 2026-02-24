#ifndef XMLBUILDER_H
#define XMLBUILDER_H

#include <string>
#include "Node.H"

namespace dom
{
	class Document;
}

class AbstractXMLBuilder
{
public:
	virtual ~AbstractXMLBuilder() {}

	virtual void addChild(const std::string &name, const std::string &value, short nodeType) = 0;
	virtual void goToChild(int index) = 0;
	virtual void goToParent() = 0;
	virtual dom::Document *getResult() = 0;
};

class DOMBuilder : public AbstractXMLBuilder
{
private:
	dom::Document *document;
	dom::Node *currentNode;

public:
	DOMBuilder();
	virtual ~DOMBuilder() {}

	virtual void addChild(const std::string &name, const std::string &value, short nodeType);
	virtual void goToChild(int index);
	virtual void goToParent();
	virtual dom::Document *getResult();
};

#endif // XMLBUILDER_H
