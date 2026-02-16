#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>

#include "Node.H"
#include "NodeList.H"

namespace dom
{
	class Element;
	class Text;
	class Attr;
	class NodeList;

	class Document : public virtual Node
	{
	public:
		virtual Element *createElement(const std::string &tagName) = 0;
		virtual Text *createTextNode(const std::string &data) = 0;
		virtual Attr *createAttribute(const std::string &name) = 0;
		virtual Element *getDocumentElement() = 0;
	};
};

class Document_Impl : public virtual dom::Document, public virtual Node_Impl
{
private:
	dom::NodeList childList;

	void setSingleChild(dom::Node *child);

public:
	Document_Impl(void);

	virtual ~Document_Impl();

	virtual dom::Element *createElement(const std::string &tagName);
	virtual dom::Text *createTextNode(const std::string &data);
	virtual dom::Attr *createAttribute(const std::string &name);
	virtual dom::Element *getDocumentElement();

	virtual dom::NodeList *getChildNodes(void);
	virtual dom::Node *getFirstChild(void);
	virtual dom::Node *getLastChild(void);
	virtual dom::Node *insertBefore(dom::Node *newChild, dom::Node *refChild);
	virtual dom::Node *replaceChild(dom::Node *newChild, dom::Node *oldChild);
	virtual dom::Node *removeChild(dom::Node *oldChild);
	virtual dom::Node *appendChild(dom::Node *newChild);
	virtual bool hasChildNodes(void);
};

#endif // DOCUMENT_H
