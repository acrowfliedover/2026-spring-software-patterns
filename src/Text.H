#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <memory>

#include "Node.H"

namespace dom
{
	class Document;

	class Text : public virtual Node
	{
	public:
		virtual ~Text()	{}

		virtual const std::string &	getName(void)							= 0;
		virtual const std::string &	getData(void)							= 0;
		virtual const std::string &	getValue(void)							= 0;
		virtual void			setData(const std::string & value)				= 0;
		virtual void			setValue(const std::string & value)				= 0;
		virtual int			getLength(void)							= 0;
		virtual const std::string &	substringData(int offset, int count)				= 0;
		virtual void			appendData(const std::string & arg)				= 0;
		virtual void			insertData(int offset, const std::string & arg)			= 0;
		virtual void			deleteData(int offset, int count)				= 0;
		virtual void			replaceData(int offset, int count, const std::string & arg)	= 0;
		virtual std::shared_ptr<Text>	splitText(int offset)						= 0;
	};
};

class Text_Impl : public virtual dom::Text, Node_Impl
{
protected:
	Text_Impl(const std::string value, dom::Document * document);
public:
	friend class Document_Impl;

	virtual ~Text_Impl();

	//
	// Visitor
	//
	virtual void accept(dom::Visitor & visitor);

	virtual const std::string &		getName(void);
	virtual const std::string &		getData(void);
	virtual const std::string &		getValue(void);
	virtual void				setData(const std::string & value);
	virtual void				setValue(const std::string & value);
	virtual int				getLength(void);
	virtual const std::string &		substringData(int offset, int count);
	virtual void				appendData(const std::string & arg);
	virtual void				insertData(int offset, const std::string & arg);
	virtual void				deleteData(int offset, int count);
	virtual void				replaceData(int offset, int count, const std::string & arg);
	virtual std::shared_ptr<dom::Text>	splitText(int offset);

	//
	// Overridden functions for conforming to Composite Leaf node behavior.
	//
	virtual std::shared_ptr<dom::Node>	insertBefore(std::shared_ptr<dom::Node> newChild,
						  std::shared_ptr<dom::Node> refChild)			{ return 0; }
	virtual std::shared_ptr<dom::Node>	replaceChild(std::shared_ptr<dom::Node> newChild,
						  std::shared_ptr<dom::Node> oldChild)			{ return 0; }
	virtual std::shared_ptr<dom::Node>	removeChild(std::shared_ptr<dom::Node> oldChild)	{ return 0; }
	virtual std::shared_ptr<dom::Node>	appendChild(std::shared_ptr<dom::Node> newChild)	{ return 0; }

	virtual std::shared_ptr<dom::Node>	cloneNode(bool deep);
};

#endif // TEXT_H
