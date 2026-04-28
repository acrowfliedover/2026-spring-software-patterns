#ifndef SERIALIZEVISITOR_H
#define SERIALIZEVISITOR_H

#include <iostream>

#include "Visitor.H"

class SerializeVisitor : public virtual dom::Visitor
{
protected:
	std::ostream *	writer;

	SerializeVisitor(std::ostream * _writer) : writer(_writer) {}

	virtual void prettyIndentation(void)	= 0;
	virtual void incrementIndentation(void)	= 0;
	virtual void decrementIndentation(void)	= 0;
	virtual void newLine(void)		= 0;

public:
	virtual ~SerializeVisitor() {}

	virtual void visit(dom::Document * document);
	virtual void visit(dom::Element * element);
	virtual void visit(dom::Attr * attr);
	virtual void visit(dom::Text * text);
};

class PrettySerializeVisitor : public SerializeVisitor
{
private:
	int	indentationLevel;

protected:
	virtual void prettyIndentation(void);
	virtual void incrementIndentation(void)	{ indentationLevel++; }
	virtual void decrementIndentation(void)	{ indentationLevel--; }
	virtual void newLine(void)		{ *writer << "\n"; }

public:
	PrettySerializeVisitor(std::ostream * _writer) :
	  SerializeVisitor(_writer),
	  indentationLevel(0) {}
};

class MinimalSerializeVisitor : public SerializeVisitor
{
protected:
	virtual void prettyIndentation(void)	{}
	virtual void incrementIndentation(void)	{}
	virtual void decrementIndentation(void)	{}
	virtual void newLine(void)		{}

public:
	MinimalSerializeVisitor(std::ostream * _writer) : SerializeVisitor(_writer) {}
};

#endif // SERIALIZEVISITOR_H
