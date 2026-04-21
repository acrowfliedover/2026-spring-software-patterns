#ifndef XMLSESSION_H
#define XMLSESSION_H

#include <memory>
#include <string>

#include "Document.H"
#include "Element.H"

class XMLSession
{
private:
	std::shared_ptr<dom::Document>	doc;
public:
	XMLSession(void);

	std::shared_ptr<dom::Document>	document(void)	{ return doc; }
	void				setDocument(std::shared_ptr<dom::Document> d)	{ doc = d; }

	std::shared_ptr<dom::Element>	findElement(const std::string & tag);
};

#endif // XMLSESSION_H
