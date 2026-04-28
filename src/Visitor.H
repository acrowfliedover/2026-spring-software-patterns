#ifndef VISITOR_H
#define VISITOR_H

namespace dom
{
	class Document;
	class Element;
	class Attr;
	class Text;

	class Visitor
	{
	public:
		virtual ~Visitor() {}

		virtual void visit(Document * document)	= 0;
		virtual void visit(Element * element)	= 0;
		virtual void visit(Attr * attr)		= 0;
		virtual void visit(Text * text)		= 0;
	};
};

#endif // VISITOR_H
