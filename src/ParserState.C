#include "Builder.H"
#include "Director.H"

namespace
{
	class BeforePrologState : public ParserState
	{
	public:
		virtual void start(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType);
	};

	class InPrologState : public ParserState
	{
	public:
		virtual void name(Director & director, Builder & builder, const std::string & text);
		virtual void attribute(Director & director, Builder & builder, const std::string & text);
		virtual void end(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType);
	};

	class OutsideTagState : public ParserState
	{
	public:
		virtual void start(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType);
		virtual void value(Director & director, Builder & builder, const std::string & text);
	};

	class InsideElementTagState : public ParserState
	{
	private:
		bool	closingElement;
	public:
		InsideElementTagState(bool _closingElement) : closingElement(_closingElement) {}

		virtual void name(Director & director, Builder & builder, const std::string & text);
		virtual void attribute(Director & director, Builder & builder, const std::string & text);
		virtual void end(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType);
	};

	class InsideAttributeState : public ParserState
	{
	private:
		std::shared_ptr<ParserState>	returnState;
	public:
		InsideAttributeState(std::shared_ptr<ParserState> _returnState) : returnState(_returnState) {}

		virtual void value(Director & director, Builder & builder, const std::string & text);
	};

	void BeforePrologState::start(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType)
	{
		if (tokenType == XMLTokenizer::XMLToken::PROLOG_START)
		{
			builder.createProlog();
			director.transitionTo(std::make_shared<InPrologState>());
		}
	}

	void InPrologState::name(Director & director, Builder & builder, const std::string & text)
	{
		builder.identifyProlog(text);
	}

	void InPrologState::attribute(Director & director, Builder & builder, const std::string & text)
	{
		builder.createAttribute(text);
		director.transitionTo(std::make_shared<InsideAttributeState>(std::make_shared<InPrologState>()));
	}

	void InPrologState::end(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType)
	{
		if (tokenType == XMLTokenizer::XMLToken::PROLOG_END || tokenType == XMLTokenizer::XMLToken::TAG_END)
		{
			builder.endProlog();
			director.transitionTo(std::make_shared<OutsideTagState>());
		}
	}

	void OutsideTagState::start(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType)
	{
		if (tokenType == XMLTokenizer::XMLToken::TAG_START)
			director.transitionTo(std::make_shared<InsideElementTagState>(false));
		else if (tokenType == XMLTokenizer::XMLToken::TAG_CLOSE_START)
			director.transitionTo(std::make_shared<InsideElementTagState>(true));
	}

	void OutsideTagState::value(Director & director, Builder & builder, const std::string & text)
	{
		builder.addValue(text);
	}

	void InsideElementTagState::name(Director & director, Builder & builder, const std::string & text)
	{
		if (closingElement)
			builder.confirmElement(text);
		else
			builder.createElement(text);
	}

	void InsideElementTagState::attribute(Director & director, Builder & builder, const std::string & text)
	{
		builder.createAttribute(text);
		director.transitionTo(std::make_shared<InsideAttributeState>(std::make_shared<InsideElementTagState>(closingElement)));
	}

	void InsideElementTagState::end(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType)
	{
		if (tokenType == XMLTokenizer::XMLToken::TAG_END)
		{
			if (closingElement)
			{
				if (!builder.popElement())
					director.transitionTo(std::shared_ptr<ParserState>());
				else
					director.transitionTo(std::make_shared<OutsideTagState>());
			}
			else
			{
				builder.pushElement();
				director.transitionTo(std::make_shared<OutsideTagState>());
			}
		}
		else if (tokenType == XMLTokenizer::XMLToken::NULL_TAG_END)
		{
			director.transitionTo(std::make_shared<OutsideTagState>());
		}
	}

	void InsideAttributeState::value(Director & director, Builder & builder, const std::string & text)
	{
		builder.valueAttribute(text);
		director.transitionTo(returnState);
	}
}

std::shared_ptr<ParserState> createBeforePrologState(void)
{
	return std::make_shared<BeforePrologState>();
}
