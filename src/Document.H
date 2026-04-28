#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <stack>
#include <memory>

#include "Node.H"
#include "ValidChildren.H"

namespace dom
{
	class Element;
	class Text;
	class Attr;
	class NodeList;

	class Iterator
	{
	public:
		virtual bool	hasNext()	= 0;
		virtual Node *	next()		= 0;
		virtual ~Iterator() {}
	};

	class Document : public virtual Node
	{
	public:
		virtual std::shared_ptr<Element>	createElement(const std::string & tagName)		= 0;
		virtual std::shared_ptr<Text>		createTextNode(const std::string & data)		= 0;
		virtual std::shared_ptr<Attr>		createAttribute(const std::string & name)		= 0;
		virtual std::shared_ptr<Element>	getDocumentElement()					= 0;

		//
		// Iterator Factory
		//
		virtual std::shared_ptr<Iterator>	createIterator(Node * node)		= 0;
	};
};

class Document_Impl : public virtual dom::Document, public virtual Node_Impl
{
public:
	Document_Impl(void);

	virtual ~Document_Impl();

	//
	// Visitor
	//
	virtual void accept(dom::Visitor & visitor);

	virtual std::shared_ptr<dom::Element>	createElement(const std::string & tagName);
	virtual std::shared_ptr<dom::Text>	createTextNode(const std::string & data);
	virtual std::shared_ptr<dom::Attr>	createAttribute(const std::string & name);
	virtual std::shared_ptr<dom::Element>	getDocumentElement();

	//
	// Iterator Factory
	//
	std::shared_ptr<dom::Iterator>		createIterator(dom::Node * node);

	virtual std::shared_ptr<dom::Node>	cloneNode(bool deep);
};

class DOMIterator : public virtual dom::Iterator
{
private:
	dom::Node *			firstNode;
	std::stack<dom::NodeList *>	listStack;
	std::stack<int>			indexStack;

	friend class Document_Impl;

	DOMIterator(dom::Node * startWithNode, Document_Impl * document);
	dom::Node * elementAt(dom::NodeList * currentList, int currentIndex);
public:
	virtual bool		hasNext();
	virtual dom::Node *	next();
};

class DocumentDecorator : public virtual dom::Document
{
};

class XMLValidator;

class DocumentValidator : public virtual DocumentDecorator, public virtual Node_Impl
{
private:
	std::shared_ptr<dom::Document>	component;
	std::shared_ptr<ValidChildren>	schemaElement;
	std::shared_ptr<XMLValidator>	validator;
public:
	DocumentValidator(dom::Document * _component, std::shared_ptr<XMLValidator> xmlValidator);

	//
	// Visitor
	//
	virtual void accept(dom::Visitor & visitor)
	  { component->accept(visitor); }

	//
	// Iterator Factory
	//
	std::shared_ptr<dom::Iterator>		createIterator(dom::Node * node)
	  { return component->createIterator(node); }
	virtual std::shared_ptr<dom::Element>	createElement(const std::string & tagName);
	//
	// Undecorated pass-through methods.
	//
	virtual std::shared_ptr<dom::Text>	createTextNode(const std::string & data) { return component->createTextNode(data); }
	virtual std::shared_ptr<dom::Attr>	createAttribute(const std::string & name){ return component->createAttribute(name);}
	virtual std::shared_ptr<dom::Element>	getDocumentElement()			 { return component->getDocumentElement(); }

	//
	// Decorated Methods
	//
	virtual std::shared_ptr<dom::Node>	insertBefore(std::shared_ptr<dom::Node> newChild,
						  std::shared_ptr<dom::Node> refChild);
	virtual std::shared_ptr<dom::Node>	replaceChild(std::shared_ptr<dom::Node> newChild,
						  std::shared_ptr<dom::Node> oldChild);
	virtual std::shared_ptr<dom::Node>	appendChild(std::shared_ptr<dom::Node> newChild);

	virtual std::shared_ptr<dom::Node>	cloneNode(bool deep);
};

#endif // DOCUMENT_H
