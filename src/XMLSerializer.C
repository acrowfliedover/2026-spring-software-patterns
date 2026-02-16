#include "XMLSerializer.H"

#include <vector>

#include "Document.H"
#include "Element.H"
#include "Attr.H"
#include "Text.H"

namespace
{
	class NullIterator : public dom::Iterator
	{
	public:
		virtual ~NullIterator() {}
		virtual void first(void) {}
		virtual void next(void) {}
		virtual bool isDone(void) const { return true; }
		virtual dom::Node *currentNode(void) const { return 0; }
		virtual dom::Iterator::Kind currentKind(void) const { return dom::Iterator::NONE; }
	};

	class TextIterator : public dom::Iterator
	{
	private:
		dom::Text *text;
		bool done;

	public:
		TextIterator(dom::Text *t) : text(t), done(true) {}

		virtual ~TextIterator() {}

		virtual void first(void)
		{
			done = (text == 0);
		}

		virtual void next(void)
		{
			done = true;
		}

		virtual bool isDone(void) const
		{
			return done;
		}

		virtual dom::Node *currentNode(void) const
		{
			return done ? 0 : text;
		}

		virtual dom::Iterator::Kind currentKind(void) const
		{
			return done ? dom::Iterator::NONE : dom::Iterator::TEXT;
		}
	};

	class XMLSerializationIterator : public dom::Iterator
	{
	private:
		struct Frame
		{
			dom::Element *element;
			dom::NodeList::iterator it;
			dom::NodeList::iterator end;
			bool entered;
			bool exited;
			bool hasChildren;

			Frame(dom::Element *e) : element(e), entered(false), exited(false), hasChildren(false)
			{
				dom::NodeList *children = element->getChildNodes();
				it = children->begin();
				end = children->end();
				hasChildren = (children->size() != 0);
			}
		};

		dom::Element *root;
		std::vector<Frame> stack;
		dom::Iterator::Kind currentKind_;
		dom::Node *currentNode_;
		bool done;

		void advance(void)
		{
			while (true)
			{
				if (stack.size() == 0)
				{
					done = true;
					currentKind_ = dom::Iterator::NONE;
					currentNode_ = 0;
					return;
				}

				Frame &f = stack.back();

				if (!f.entered)
				{
					f.entered = true;
					currentKind_ = dom::Iterator::ENTER_ELEMENT;
					currentNode_ = f.element;

					if (!f.hasChildren)
						f.exited = true;

					return;
				}

				if (!f.exited && f.hasChildren)
				{
					while (f.it != f.end)
					{
						dom::Node *child = *f.it;
						++f.it;

						dom::Element *childElement = dynamic_cast<dom::Element *>(child);
						if (childElement != 0)
						{
							stack.push_back(Frame(childElement));
							goto continue_outer;
						}

						dom::Text *childText = dynamic_cast<dom::Text *>(child);
						if (childText != 0)
						{
							currentKind_ = dom::Iterator::TEXT;
							currentNode_ = childText;
							return;
						}
					}

					f.exited = true;
					currentKind_ = dom::Iterator::EXIT_ELEMENT;
					currentNode_ = f.element;
					return;
				}

				stack.pop_back();

			continue_outer:;
			}
		}

	public:
		XMLSerializationIterator(dom::Element *r) : root(r), done(true) {}

		virtual ~XMLSerializationIterator() {}

		virtual void first(void)
		{
			stack.clear();
			currentKind_ = dom::Iterator::NONE;
			currentNode_ = 0;

			if (root == 0)
			{
				done = true;
				return;
			}

			done = false;
			stack.push_back(Frame(root));
			advance();
		}

		virtual void next(void)
		{
			if (done)
				return;
			advance();
		}

		virtual bool isDone(void) const
		{
			return done;
		}

		virtual dom::Node *currentNode(void) const
		{
			return currentNode_;
		}

		virtual dom::Iterator::Kind currentKind(void) const
		{
			return currentKind_;
		}
	};
};

void XMLSerializer::prettyIndentation()
{
	for (int i = 0; i < indentationLevel; i++)
		file << "\t";
}

void XMLSerializer::serializePretty(dom::Node *node)
{
	if (node == 0)
		return;

	indentationLevel = 0;

	if (dynamic_cast<dom::Document *>(node) != 0)
	{
		file << "<? xml version=\"1.0\" encoding=\"UTF-8\"?>";
		file << "\n";
	}

	dom::Iterator *it = node->createIterator();
	if (it == 0)
		return;

	for (it->first(); !it->isDone(); it->next())
	{
		if (it->currentKind() == dom::Iterator::ENTER_ELEMENT)
		{
			dom::Element *element = dynamic_cast<dom::Element *>(it->currentNode());

			prettyIndentation();
			file << "<" << element->getTagName();

			int attrCount = 0;

			for (dom::NamedNodeMap::iterator i = element->getAttributes()->begin();
				 i != element->getAttributes()->end();
				 i++)
			{
				dom::Attr *attr = dynamic_cast<dom::Attr *>(*i);
				if (attr != 0)
				{
					file << " " << attr->getName() << "=\"" << attr->getValue() << "\"";
					attrCount++;
				}
			}

			if (attrCount > 0)
				file << " ";

			if (element->getChildNodes()->size() == 0)
			{
				file << "/>";
				file << "\n";
			}
			else
			{
				file << ">";
				file << "\n";
				indentationLevel++;
			}
		}
		else if (it->currentKind() == dom::Iterator::TEXT)
		{
			dom::Text *text = dynamic_cast<dom::Text *>(it->currentNode());

			prettyIndentation();
			file << text->getData();
			file << "\n";
		}
		else if (it->currentKind() == dom::Iterator::EXIT_ELEMENT)
		{
			dom::Element *element = dynamic_cast<dom::Element *>(it->currentNode());

			indentationLevel--;
			prettyIndentation();
			file << "</" << element->getTagName() << ">";
			file << "\n";
		}
	}

	delete it;
}

void XMLSerializer::serializeMinimal(dom::Node *node)
{
	if (node == 0)
		return;

	if (dynamic_cast<dom::Document *>(node) != 0)
		file << "<? xml version=\"1.0\" encoding=\"UTF-8\"?>";

	dom::Iterator *it = node->createIterator();
	if (it == 0)
		return;

	for (it->first(); !it->isDone(); it->next())
	{
		if (it->currentKind() == dom::Iterator::ENTER_ELEMENT)
		{
			dom::Element *element = dynamic_cast<dom::Element *>(it->currentNode());

			file << "<" << element->getTagName();

			for (dom::NamedNodeMap::iterator i = element->getAttributes()->begin();
				 i != element->getAttributes()->end();
				 i++)
			{
				dom::Attr *attr = dynamic_cast<dom::Attr *>(*i);
				if (attr != 0)
					file << " " << attr->getName() << "=\"" << attr->getValue() << "\"";
			}

			if (element->getChildNodes()->size() == 0)
				file << "/>";
			else
				file << ">";
		}
		else if (it->currentKind() == dom::Iterator::TEXT)
		{
			dom::Text *text = dynamic_cast<dom::Text *>(it->currentNode());
			file << text->getData();
		}
		else if (it->currentKind() == dom::Iterator::EXIT_ELEMENT)
		{
			dom::Element *element = dynamic_cast<dom::Element *>(it->currentNode());
			file << "</" << element->getTagName() << ">";
		}
	}

	delete it;
}

dom::Iterator *Node_Impl::createIterator(void)
{
	if (dynamic_cast<dom::Document *>(this) != 0)
	{
		dom::Element *root = dynamic_cast<dom::Document *>(this)->getDocumentElement();
		return root ? (dom::Iterator *)new XMLSerializationIterator(root) : (dom::Iterator *)new NullIterator();
	}

	if (dynamic_cast<dom::Element *>(this) != 0)
		return (dom::Iterator *)new XMLSerializationIterator(dynamic_cast<dom::Element *>(this));

	if (dynamic_cast<dom::Text *>(this) != 0)
		return (dom::Iterator *)new TextIterator(dynamic_cast<dom::Text *>(this));

	return (dom::Iterator *)new NullIterator();
}
