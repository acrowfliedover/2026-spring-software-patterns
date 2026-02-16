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
	};

	class XMLSerializationIterator : public dom::Iterator
	{
	private:
		dom::Element *root;
		std::vector<dom::Node *> stack;
		std::vector<char> stackEvent; // 0 = normal/open, 1 = closing event for Element
		dom::Node *currentNode_;
		char currentEvent_;
		bool done;

		void advance(void)
		{
			if (done || currentNode_ == 0)
			{
				done = true;
				currentNode_ = 0;
				return;
			}

			// After yielding an opening Element that has children, schedule:
			//   children in order, then the same Element again for closing.
			if (currentEvent_ == 0)
			{
				dom::Element *elem = dynamic_cast<dom::Element *>(currentNode_);
				if (elem != 0 && elem->getFirstChild() != 0)
				{
					stack.push_back(elem);
					stackEvent.push_back(1);

					std::vector<dom::Node *> kids;
					for (dom::Node *c = elem->getFirstChild(); c != 0; c = c->getNextSibling())
						kids.push_back(c);

					for (int i = (int)kids.size() - 1; i >= 0; --i)
					{
						stack.push_back(kids[i]);
						stackEvent.push_back(0);
					}
				}
			}

			if (stack.size() == 0)
			{
				done = true;
				currentNode_ = 0;
				return;
			}

			currentNode_ = stack.back();
			currentEvent_ = stackEvent.back();
			stack.pop_back();
			stackEvent.pop_back();
		}

	public:
		XMLSerializationIterator(dom::Element *r) : root(r), currentNode_(0), currentEvent_(0), done(true) {}

		virtual ~XMLSerializationIterator() {}

		virtual void first(void)
		{
			stack.clear();
			stackEvent.clear();
			currentNode_ = 0;
			currentEvent_ = 0;

			if (root == 0)
			{
				done = true;
				return;
			}

			done = false;
			currentNode_ = root;
			currentEvent_ = 0;
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

	std::vector<dom::Element *> elementStack;

	for (it->first(); !it->isDone(); it->next())
	{
		dom::Node *n = it->currentNode();
		dom::Element *element = dynamic_cast<dom::Element *>(n);

		if (element != 0)
		{
			if (elementStack.size() > 0 && elementStack.back() == element)
			{
				indentationLevel--;
				prettyIndentation();
				file << "</" << element->getTagName() << ">";
				file << "\n";
				elementStack.pop_back();
			}
			else
			{
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
					elementStack.push_back(element);
				}
			}
		}
		else
		{
			dom::Text *text = dynamic_cast<dom::Text *>(n);
			if (text != 0)
			{
				prettyIndentation();
				file << text->getData();
				file << "\n";
			}
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

	std::vector<dom::Element *> elementStack;

	for (it->first(); !it->isDone(); it->next())
	{
		dom::Node *n = it->currentNode();
		dom::Element *element = dynamic_cast<dom::Element *>(n);

		if (element != 0)
		{
			if (elementStack.size() > 0 && elementStack.back() == element)
			{
				file << "</" << element->getTagName() << ">";
				elementStack.pop_back();
			}
			else
			{
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
				{
					file << ">";
					elementStack.push_back(element);
				}
			}
		}
		else
		{
			dom::Text *text = dynamic_cast<dom::Text *>(n);
			if (text != 0)
				file << text->getData();
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
