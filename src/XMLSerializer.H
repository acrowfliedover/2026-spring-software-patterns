#include <string>
#include <fstream>
#include <ios>

#include "Node.H"

//
// Iterator pattern roles (kept in XMLSerializer files per assignment request):
// - Iterator interface: dom::Iterator
// - Concrete Iterator: implemented in XMLSerializer.C
//
namespace dom
{
	class Iterator
	{
	public:
		enum Kind
		{
			NONE = 0,
			ENTER_ELEMENT,
			EXIT_ELEMENT,
			TEXT
		};

		virtual ~Iterator() {}
		virtual void first(void) = 0;
		virtual void next(void) = 0;
		virtual bool isDone(void) const = 0;
		virtual Node *currentNode(void) const = 0;
		virtual Kind currentKind(void) const = 0;
	};
};

class XMLSerializer
{
private:
	std::fstream file;
	int indentationLevel;

	virtual void prettyIndentation();

public:
	XMLSerializer(const std::string &filename) : indentationLevel(0), file(filename.c_str(), std::ios_base::out) {}
	~XMLSerializer() {}

	virtual void serializePretty(dom::Node *node);
	virtual void serializeMinimal(dom::Node *node);
};
