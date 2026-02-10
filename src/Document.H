#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>

#include "Node.H"

namespace dom
{
	class Element;
	class Text;
	class Attr;
	class NodeList;

	class Document : public virtual Node // abstract creator class
	{
	public:
		virtual Element *createElement(const std::string &tagName) = 0; //abstract factory method
		virtual Text *createTextNode(const std::string &data) = 0; //abstract factory method
		virtual Attr *createAttribute(const std::string &name) = 0; //abstract factory method
		virtual Element *getDocumentElement() = 0;
	};
};

class Document_Impl : public virtual dom::Document, public virtual Node_Impl 
// concrete creator class
{
public:
	Document_Impl(void);

	virtual ~Document_Impl();

	virtual dom::Element *createElement(const std::string &tagName); // concrete factory method
	virtual dom::Text *createTextNode(const std::string &data); // concrete factory method
	virtual dom::Attr *createAttribute(const std::string &name); // concrete factory method
	virtual dom::Element *getDocumentElement();
	virtual void serializeWith(class XMLSerializer & s);
};

#endif // DOCUMENT_H
