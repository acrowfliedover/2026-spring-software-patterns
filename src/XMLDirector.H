#ifndef XMLDIRECTOR_H
#define XMLDIRECTOR_H

#include "XMLBuilder.H"
#include "XMLTokenizer.H"

class XMLDirector
{
private:
	AbstractXMLBuilder *builder;

public:
	XMLDirector(AbstractXMLBuilder *builder);
	virtual ~XMLDirector() {}

	virtual void construct(XMLTokenizer &tokenizer);
};

#endif // XMLDIRECTOR_H
