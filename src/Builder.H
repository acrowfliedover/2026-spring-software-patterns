#ifndef BUILDER_H
#define BUILDER_H

#include <memory>
#include <stack>
#include <string>
#include "ParseObserver.H"

class ParseMediator;

namespace dom
{
	class Document;
	class Element;
	class Attr;
};

class Builder
{
private:
	std::shared_ptr<dom::Document>			factory;
	std::stack<std::shared_ptr<dom::Element>>	elementStack;
	std::shared_ptr<dom::Element>			currentElement;
	std::shared_ptr<dom::Attr>			currentAttr;
	std::shared_ptr<ParseMediator>			changeManager;
	int						depth;

	virtual const std::string	trim(const std::string & s) const;
public:
	Builder(std::shared_ptr<dom::Document> _factory, std::shared_ptr<ParseMediator> _changeManager = std::shared_ptr<ParseMediator>());

	virtual std::shared_ptr<dom::Document>	getDocument(void)	{ return factory; }
	virtual void		attach(std::shared_ptr<ParseObserver> observer);
	virtual void		detach(std::shared_ptr<ParseObserver> observer);
	virtual void		notify(const ParseEvent & event);

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
