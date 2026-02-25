#ifndef NAMEDNODEMAP_H
#define NAMEDNODEMAP_H

#include <string>
#include <memory>

#include "NodeList.H"

namespace dom
{
	class Document;

	class NamedNodeMap : public NodeList
	{
	public:
		virtual std::shared_ptr<Node>	getNamedItem(const std::string & name)		{ return 0; }
		virtual std::shared_ptr<Node>	setNamedItem(std::shared_ptr<Node> arg)		{ return 0; }
		virtual std::shared_ptr<Node>	removeNamedItem(const std::string & name)	{ return 0; }
	};
};

class NamedNodeMap_Impl : public dom::NamedNodeMap
{
private:
	dom::Document *	document;
public:
	NamedNodeMap_Impl(dom::Document * document);

	virtual ~NamedNodeMap_Impl();

	virtual std::shared_ptr<dom::Node>	getNamedItem(const std::string & name);
	virtual std::shared_ptr<dom::Node>	setNamedItem(std::shared_ptr<dom::Node> arg);
	virtual std::shared_ptr<dom::Node>	removeNamedItem(const std::string & name);
};

#endif // NAMEDNODEMAP_H
