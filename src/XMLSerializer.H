#include <iostream>
#include <memory>

#include "Node.H"
#include "WhitespaceStrategy.H"

//
// XMLSerializer is Serialization Visitor Context
// java.io.Writer is output stream Strategy
// java.io.BufferedWriter is output stream Concrete Strategy
//
class XMLSerializer
{
private:
	std::ostream *		file;
public:
	XMLSerializer(std::ostream * _file) : file(_file)	{}
	~XMLSerializer() {}
private:
	class PrettyWhitespaceStrategy : public WhitespaceStrategy
	{
	private:
		int	indentationLevel;
	public:
		PrettyWhitespaceStrategy(void) : indentationLevel(0)	{}

		virtual void	prettyIndentation(std::ostream * wwriter)
		{
			for (int i = 0; i < indentationLevel; i++)
				*wwriter << "\t";
		}

		virtual void incrementIndentation(void)		{ indentationLevel++; }
		virtual void decrementIndentation(void)		{ indentationLevel--; }
		virtual void newLine(std::ostream * wwriter)	{ *wwriter << "\n"; }
	};

	class MinimalWhitespaceStrategy : public WhitespaceStrategy
	{
	public:
		virtual void prettyIndentation(std::ostream * wwriter)	{}
		virtual void incrementIndentation(void)			{}
		virtual void decrementIndentation(void)			{}
		virtual void newLine(std::ostream * wwriter)		{}
	};
public:
	//
	// Visitor-based serialization
	//
	virtual void		serializePretty(std::shared_ptr<dom::Node> node);
	virtual void		serializeMinimal(std::shared_ptr<dom::Node> node);
};
