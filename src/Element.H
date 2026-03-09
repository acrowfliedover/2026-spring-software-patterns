#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <memory>

#include "Node.H"
#include "NamedNodeMap.H"
#include "ValidChildren.H"

namespace dom
{
	class Attr;
	class NodeList;
	class Document;

	class Element : public virtual Node
	{
	public:
		virtual ~Element() {}

		//
		// Serialization Data Extraction Strategy
		//
		virtual void serialize(std::fstream * writer, std::shared_ptr<WhitespaceStrategy> whitespace)		= 0;

		virtual const std::string &		getAttribute(const std::string & name)				= 0;
		virtual std::shared_ptr<Attr>		getAttributeNode(const std::string & name)			= 0;
		virtual std::shared_ptr<NodeList>	getElementsByTagName(const std::string & tagName)		= 0;
		virtual const std::string &		getTagName(void)						= 0;
		virtual bool				hasAttribute(const std::string & name)				= 0;
		virtual void				removeAttribute(const std::string & name)			= 0;
		virtual std::shared_ptr<Attr>		removeAttributeNode(std::shared_ptr<Attr> oldAttr)		= 0;
		virtual void				setAttribute(const std::string & name,const std::string& value)	= 0;
		virtual std::shared_ptr<Attr>		setAttributeNode(std::shared_ptr<Attr> newAttr)			= 0;

		virtual dom::NamedNodeMap *		getAttributes(void)						= 0;
		virtual bool				hasAttributes(void)						= 0;
	};
};

class Element_Impl : public virtual dom::Element, Node_Impl
{
private:
	NamedNodeMap_Impl	attributes;
protected:
	Element_Impl(const std::string & tagName, dom::Document * document);
public:
	friend class Document_Impl;

	virtual ~Element_Impl();

	//
	// Serialization Data Extraction Strategy
	//
	virtual void serialize(std::fstream * writer, std::shared_ptr<WhitespaceStrategy> whitespace);

	virtual const std::string &		getAttribute(const std::string & name);
	virtual std::shared_ptr<dom::Attr>	getAttributeNode(const std::string & name);
	virtual std::shared_ptr<dom::NodeList>	getElementsByTagName(const std::string & tagName);
	virtual const std::string &		getTagName(void);
	virtual bool				hasAttribute(const std::string & name);
	virtual void				removeAttribute(const std::string & name);
	virtual std::shared_ptr<dom::Attr>	removeAttributeNode(std::shared_ptr<dom::Attr> oldAttr);
	virtual void				setAttribute(const std::string & name, const std::string & value);
	virtual std::shared_ptr<dom::Attr>	setAttributeNode(std::shared_ptr<dom::Attr> newAttr);

	virtual dom::NamedNodeMap *		getAttributes(void)	{ return &attributes; }
	virtual bool				hasAttributes(void)	{ return attributes.size() > 0; }
};

class ElementDecorator : public virtual dom::Element
{
};

class XMLValidator;
namespace dom
{
	class NodeList;
};

class ElementValidator : public virtual ElementDecorator, public virtual Node_Impl
{
private:
	std::shared_ptr<dom::Element>	component;
	std::shared_ptr<ValidChildren>	schemaElement;
public:
	ElementValidator(std::shared_ptr<dom::Element> _component, std::shared_ptr<XMLValidator> xmlValidator);

	//
	// Serialization Data Extraction Strategy
	//
	virtual void serialize(std::fstream * writer, std::shared_ptr<WhitespaceStrategy> whitespace)
	  { component->serialize(writer, whitespace); }

	//
	// Undecorated pass-through methods.
	//
	virtual const std::string &	getAttribute(const std::string & name)		{ return component->getAttribute(name); }
	virtual std::shared_ptr<dom::Attr>	getAttributeNode(const std::string & name)
	  { return component->getAttributeNode(name); }
	virtual std::shared_ptr<dom::NodeList>	getElementsByTagName(const std::string & tagName)
	  { return component->getElementsByTagName(tagName); }
	virtual const std::string &	getTagName(void)				{ return component->getTagName(); }
	virtual bool			hasAttribute(const std::string & name)		{ return component->hasAttribute(name); }
	virtual void			removeAttribute(const std::string & name)	{ component->removeAttribute(name); }
	virtual std::shared_ptr<dom::Attr>	removeAttributeNode(std::shared_ptr<dom::Attr> oldAttr)
	  { return component->removeAttributeNode(oldAttr); }

	//
	// Decorated Methods
	//
	virtual void				setAttribute(const std::string & name, const std::string & value);
	virtual std::shared_ptr<dom::Attr>	setAttributeNode(std::shared_ptr<dom::Attr> newAttr);
	virtual std::shared_ptr<dom::Node>	insertBefore(std::shared_ptr<dom::Node> newChild,
						  std::shared_ptr<dom::Node> refChild);
	virtual std::shared_ptr<dom::Node>	replaceChild(std::shared_ptr<dom::Node> newChild,
						  std::shared_ptr<dom::Node> oldChild);
	virtual std::shared_ptr<dom::Node>	appendChild(std::shared_ptr<dom::Node> newChild);

	//
	// Reimplemented Node members
	//
	virtual dom::NamedNodeMap *		getAttributes(void)			{ return component->getAttributes(); }
	virtual bool				hasAttributes(void)			{ return component->hasAttributes(); }
	virtual const std::string &		getNodeName(void)			{ return component->getNodeName(); }
	virtual const std::string &		getNodeValue(void)			{ return component->getNodeValue(); }
	virtual void				setNodeValue(const std::string & nodeValue) { component->setNodeValue(nodeValue); }
	virtual short				getNodeType(void)			{ return component->getNodeType(); }
	virtual dom::Node *			getParentNode(void)			{ return component->getParentNode(); }
	virtual dom::NodeList *			getChildNodes(void)			{ return component->getChildNodes(); }
	virtual std::shared_ptr<dom::Node>	getFirstChild(void)			{ return component->getFirstChild(); }
	virtual std::shared_ptr<dom::Node>	getLastChild(void)			{ return component->getLastChild(); }
	virtual std::shared_ptr<dom::Node>	getPreviousSibling(void)		{ return component->getPreviousSibling(); }
	virtual std::shared_ptr<dom::Node>	getNextSibling(void)			{ return component->getNextSibling(); }
	virtual dom::Document *			getOwnerDocument(void)			{ return component->getOwnerDocument(); }
	virtual std::shared_ptr<dom::Node>	removeChild(std::shared_ptr<Node> oldChild) { return component->removeChild(oldChild);}
	virtual bool				hasChildNodes(void)			{ return component->hasChildNodes(); }
	virtual const std::string &		getLocalName(void)			{ return component->getLocalName(); }
};

class Director;

class ElementProxy : public virtual dom::Element
{
private:
	dom::Element *	realSubject;
	bool		realized;
	Director *	director;

	void	realize(void);
public:
	ElementProxy(dom::Element * _realSubject, Director * _director) :
	  realSubject(_realSubject),
	  director(_director),
	  realized(false)
	{
	}
	virtual ~ElementProxy() {}

	virtual dom::NodeList *			getChildNodes(void);
	virtual std::shared_ptr<dom::Node>	getFirstChild(void);
	virtual std::shared_ptr<dom::Node>	getLastChild(void);
	virtual bool				hasChildNodes(void);

	//
	// Passthrough Element members.
	//
	virtual void serialize(std::fstream * writer, std::shared_ptr<WhitespaceStrategy> whitespace)
	  { realSubject->serialize(writer, whitespace); }

	virtual const std::string &		getAttribute(const std::string & name)	{ return realSubject->getAttribute(name); }
	virtual std::shared_ptr<dom::Attr>	getAttributeNode(const std::string & name)
	  { return realSubject->getAttributeNode(name); }
	virtual std::shared_ptr<dom::NodeList>	getElementsByTagName(const std::string & tagName)
	  { return realSubject->getElementsByTagName(tagName); }
	virtual const std::string &	getTagName(void)				{ return realSubject->getTagName(); }
	virtual bool			hasAttribute(const std::string & name)		{ return realSubject->hasAttribute(name); }
	virtual void			removeAttribute(const std::string & name)	{ realSubject->removeAttribute(name); }
	virtual std::shared_ptr<dom::Attr>	removeAttributeNode(std::shared_ptr<dom::Attr> oldAttr)
	  {return realSubject->removeAttributeNode(oldAttr);}
	virtual void				setAttribute(const std::string & name, const std::string & value)
	  { realSubject->setAttribute(name, value); }
	virtual std::shared_ptr<dom::Attr>	setAttributeNode(std::shared_ptr<dom::Attr> newAttr)
	  { return realSubject->setAttributeNode(newAttr); }

	virtual dom::NamedNodeMap *	getAttributes(void)				{ return realSubject->getAttributes(); }
	virtual bool			hasAttributes(void)				{ return realSubject->hasAttributes(); }
	virtual const std::string &	getNodeName(void)				{ return realSubject->getNodeName(); }
	virtual const std::string &	getNodeValue(void)				{ return realSubject->getNodeValue(); }
	virtual void			setNodeValue(const std::string & nodeValue)	{ realSubject->setNodeValue(nodeValue); }
	virtual short			getNodeType(void)				{ return realSubject->getNodeType(); }
	virtual dom::Node *		getParentNode(void)				{ return realSubject->getParentNode(); }
	virtual std::shared_ptr<dom::Node>	getPreviousSibling(void)		{ return realSubject->getPreviousSibling();}
	virtual std::shared_ptr<dom::Node>	getNextSibling(void)			{ return realSubject->getNextSibling(); }
	virtual dom::Document *		getOwnerDocument(void)				{ return realSubject->getOwnerDocument(); }
	virtual std::shared_ptr<dom::Node>	removeChild(std::shared_ptr<dom::Node> oldChild)
	  {return realSubject->removeChild(oldChild);}
	virtual const std::string &	getLocalName(void)				{ return realSubject->getLocalName(); }
};

#endif // ELEMENT_H
