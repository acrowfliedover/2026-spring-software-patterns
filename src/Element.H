#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <memory>

#include "Node.H"
#include "NamedNodeMap.H"
#include "ValidChildren.H"
#include "XMLTokenizer.H"

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

class Element_Impl : public virtual dom::Element, public Node_Impl
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

class ElementProxy_Impl : public Element_Impl
{
private:
	std::shared_ptr<dom::Document>	factory;
	std::string			sourceFilename;
	XMLTokenizer::TokenizerState	childTokenizerState;
	bool				hasDeferredChildren;
	bool				childrenLoaded;
	bool				loadingChildren;

	static std::string		trim(const std::string & s);
	static std::string		getAttributeName(const std::string & token);
	static std::string		getAttributeValue(const std::string & token);
	static void			skipElementBody(XMLTokenizer & tokenizer);
	void				materializeChildren();
public:
	ElementProxy_Impl(const std::string & tagName, std::shared_ptr<dom::Document> documentFactory,
	  const std::string & filename);

	virtual ~ElementProxy_Impl();

	static std::shared_ptr<dom::Element>	createProxyElement(std::shared_ptr<dom::Document> documentFactory,
	  const std::string & filename, XMLTokenizer & tokenizer, const std::string & tagToken);

	//
	// Child-sensitive methods trigger materialization.
	//
	virtual void				serialize(std::fstream * writer, std::shared_ptr<WhitespaceStrategy> whitespace);
	virtual std::shared_ptr<dom::NodeList>	getElementsByTagName(const std::string & tagName);
	virtual dom::NodeList *			getChildNodes(void);
	virtual std::shared_ptr<dom::Node>	getFirstChild(void);
	virtual std::shared_ptr<dom::Node>	getLastChild(void);
	virtual std::shared_ptr<dom::Node>	getPreviousSibling(void);
	virtual std::shared_ptr<dom::Node>	getNextSibling(void);
	virtual std::shared_ptr<dom::Node>	insertBefore(std::shared_ptr<dom::Node> newChild,
					  std::shared_ptr<dom::Node> refChild);
	virtual std::shared_ptr<dom::Node>	replaceChild(std::shared_ptr<dom::Node> newChild,
					  std::shared_ptr<dom::Node> oldChild);
	virtual std::shared_ptr<dom::Node>	removeChild(std::shared_ptr<dom::Node> oldChild);
	virtual std::shared_ptr<dom::Node>	appendChild(std::shared_ptr<dom::Node> newChild);
	virtual bool				hasChildNodes(void);
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

#endif // ELEMENT_H
