#ifndef SERIALIZEVISITOR_H
#define SERIALIZEVISITOR_H

#include <iostream>
#include <memory>

#include "Visitor.H"
#include "WhitespaceStrategy.H"

class SerializeVisitor : public virtual dom::Visitor
{
private:
	std::ostream *				writer;
	std::shared_ptr<WhitespaceStrategy>	whitespace;

public:
	SerializeVisitor(std::ostream * _writer, std::shared_ptr<WhitespaceStrategy> _whitespace) :
	  writer(_writer),
	  whitespace(_whitespace) {}

	virtual void visit(dom::Document * document);
	virtual void visit(dom::Element * element);
	virtual void visit(dom::Attr * attr);
	virtual void visit(dom::Text * text);
};

#endif // SERIALIZEVISITOR_H
