#ifndef ATTR_H
#define ATTR_H

#include <string>
#include <memory>

#include "Node.H"

namespace dom
{
	class Document;
	class Element;

	class Attr : public virtual Node
	{
	public:
		virtual ~Attr() {}

		virtual const std::string &	getName(void)				= 0;
		virtual const std::string &	getValue(void)				= 0;
		virtual void			setValue(const std::string & value)	= 0;
		virtual Element *		getOwnerElement(void)			= 0;
	};
};

class Attr_Impl : public virtual dom::Attr, Node_Impl
{
protected:
	Attr_Impl(const std::string & tagName, dom::Document * document);
	Attr_Impl(const std::string & tagName, const std::string & value, dom::Document * document);
public:
	friend class Document_Impl;
	friend class Element_Impl;

	virtual ~Attr_Impl();

	//
	// Visitor
	//
	virtual void accept(dom::Visitor & visitor);

	virtual const std::string &		getName(void);
	virtual const std::string &		getValue(void);
	virtual void				setValue(const std::string & value);
	virtual dom::Element *			getOwnerElement(void);

	//
	// Overridden functions for conforming to Composite Leaf node behavior.
	//
	virtual std::shared_ptr<Node>		insertBefore(Node * newChild, Node * refChild)
	  { return std::shared_ptr<Attr_Impl>(); }
	virtual std::shared_ptr<Node>		replaceChild(Node * newChild, Node * oldChild)
	  { return std::shared_ptr<Attr_Impl>(); }
	virtual std::shared_ptr<Node>		removeChild(Node * oldChild)
	  { return std::shared_ptr<Attr_Impl>(); }
	virtual std::shared_ptr<Node>		appendChild(Node * newChild)
	  { return std::shared_ptr<Attr_Impl>(); }

	virtual std::shared_ptr<dom::Node>	cloneNode(bool deep);
};

#endif // ATTR_H
