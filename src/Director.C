#include "Director.H"
#include "Builder.H"

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

Director::Director(const std::string & filename, std::shared_ptr<Builder> _builder) :
  builder(_builder)
{
	XMLTokenizer	tokenizer(filename);
	std::shared_ptr<XMLTokenizer::XMLToken>	token;

	transitionTo(std::make_shared<BeforePrologState>());

	do
	{
		token		= tokenizer.getNextToken();
		dispatch(token);
	} while(token->getTokenType() != XMLTokenizer::XMLToken::NULL_TOKEN);
}

void Director::transitionTo(std::shared_ptr<ParserState> nextState)
{
	state	= nextState;
}

void Director::dispatch(std::shared_ptr<XMLTokenizer::XMLToken> token)
{
	if (state == 0 || builder == 0)
		return;

	switch(token->getTokenType())
	{
	case XMLTokenizer::XMLToken::PROLOG_START:
	case XMLTokenizer::XMLToken::TAG_START:
	case XMLTokenizer::XMLToken::TAG_CLOSE_START:
		state->start(*this, *builder, token->getTokenType());
		break;
	case XMLTokenizer::XMLToken::PROLOG_IDENTIFIER:
	case XMLTokenizer::XMLToken::ELEMENT:
		state->name(*this, *builder, token->getToken());
		break;
	case XMLTokenizer::XMLToken::ATTRIBUTE:
		state->attribute(*this, *builder, token->getToken());
		break;
	case XMLTokenizer::XMLToken::ATTRIBUTE_VALUE:
	case XMLTokenizer::XMLToken::VALUE:
		state->value(*this, *builder, token->getToken());
		break;
	case XMLTokenizer::XMLToken::PROLOG_END:
	case XMLTokenizer::XMLToken::TAG_END:
	case XMLTokenizer::XMLToken::NULL_TAG_END:
	case XMLTokenizer::XMLToken::NULL_TOKEN:
		state->end(*this, *builder, token->getTokenType());
		break;
	default:
		break;
	}
}
