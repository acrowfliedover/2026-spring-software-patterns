#ifndef NODEFLYWEIGHT_H
#define NODEFLYWEIGHT_H

#include <map>
#include <memory>
#include <string>
#include <utility>

namespace dom
{
	//
	// Flyweight: intrinsic state (node kind + name) shared across nodes.
	//
	class NodeFlyweight
	{
	public:
		virtual ~NodeFlyweight() {}

		virtual short			getNodeType(void) const			= 0;
		virtual const std::string &	getName(void) const			= 0;
	};

	//
	// ConcreteFlyweight: element tag name is intrinsic.
	//
	class ElementFlyweight : public NodeFlyweight
	{
	private:
		std::string	tagName;
	public:
		explicit ElementFlyweight(const std::string & n);

		virtual short			getNodeType(void) const;
		virtual const std::string &	getName(void) const;
	};

	//
	// ConcreteFlyweight: attribute local name is intrinsic.
	//
	class AttrFlyweight : public NodeFlyweight
	{
	private:
		std::string	attrName;
	public:
		explicit AttrFlyweight(const std::string & n);

		virtual short			getNodeType(void) const;
		virtual const std::string &	getName(void) const;
	};

	//
	// ConcreteFlyweight: single pooled instance per (TEXT_NODE, "") — name always empty.
	//
	class TextFlyweight : public NodeFlyweight
	{
	private:
		static const std::string	emptyName;
	public:
		TextFlyweight(void);

		virtual short			getNodeType(void) const;
		virtual const std::string &	getName(void) const;
	};

	//
	// UnsharedConcreteFlyweight in role terms: document node is unique per tree; intrinsic
	// state (DOCUMENT_NODE, "") is still one shared flyweight in the pool keyed by type+name.
	//
	class DocumentFlyweight : public NodeFlyweight
	{
	private:
		static const std::string	emptyName;
	public:
		DocumentFlyweight(void);

		virtual short			getNodeType(void) const;
		virtual const std::string &	getName(void) const;
	};

	//
	// FlyweightFactory
	//
	class NodeFlyweightFactory
	{
	private:
		using Key	= std::pair<short, std::string>;
		std::map<Key, std::shared_ptr<const NodeFlyweight>>	pool;

		NodeFlyweightFactory(void) {}
		NodeFlyweightFactory(const NodeFlyweightFactory &) = delete;
		NodeFlyweightFactory & operator=(const NodeFlyweightFactory &) = delete;

		std::shared_ptr<const NodeFlyweight>	createFlyweight(short nodeType,
						  const std::string & name);
	public:
		static NodeFlyweightFactory &	instance(void);

		std::shared_ptr<const NodeFlyweight>	getFlyweight(short nodeType, const std::string & name);
	};
};

#endif // NODEFLYWEIGHT_H
