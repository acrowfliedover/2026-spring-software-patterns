#ifndef BUILDER_H
#define BUILDER_H

#include "Subject.H"
#include <stack>
#include <string>

class Observer;

namespace dom
{
	class Document;
	class Element;
	class Attr;
};

class Builder : public Subject
{
private:
	std::shared_ptr<dom::Document>			factory;
	std::stack<std::shared_ptr<dom::Element>>	elementStack;
	std::shared_ptr<dom::Element>			currentElement;
	std::shared_ptr<dom::Attr>			currentAttr;

	virtual const std::string	trim(const std::string & s) const;
public:
	Builder(std::shared_ptr<dom::Document> _factory, std::shared_ptr<Observer> _observer) :
	  factory(_factory), currentElement(nullptr), currentAttr(nullptr) { if (_observer) attach(_observer); }

	virtual std::shared_ptr<dom::Document>	getDocument(void)	{ return factory; }	// aka getResult

	virtual void		addValue(const std::string & text);
	virtual void		confirmElement(const std::string & tag);
	virtual void		createAttribute(const std::string & attribute);
	virtual void		createElement(const std::string & tag);
	virtual void		createProlog(void);
	virtual void		endProlog(void);
	virtual void		identifyProlog(const std::string & id);
	virtual bool		popElement(void);
	virtual void		pushElement(void);
	virtual void		valueAttribute(const std::string & value);
};

#endif // BUILDER_H
