#include "Director.H"
#include "Builder.H"

namespace
{
	class BeforePrologState : public ParserState
	{
	public:
		virtual void start(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType)
		{
			if (tokenType == XMLTokenizer::XMLToken::PROLOG_START)
			{
				builder.createProlog();
				director.transitionTo(director.getInPrologState());
			}
		}
	};

	class InPrologState : public ParserState
	{
	public:
		virtual void name(Director & director, Builder & builder, const std::string & text)
		{
			builder.identifyProlog(text);
		}

		virtual void attribute(Director & director, Builder & builder, const std::string & text)
		{
			builder.createAttribute(text);
			director.transitionToAttribute(director.getInPrologState());
		}

		virtual void end(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType)
		{
			if (tokenType == XMLTokenizer::XMLToken::PROLOG_END || tokenType == XMLTokenizer::XMLToken::TAG_END)
			{
				builder.endProlog();
				director.transitionTo(director.getOutsideTagState());
			}
		}
	};

	class OutsideTagState : public ParserState
	{
	public:
		virtual void start(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType)
		{
			if (tokenType == XMLTokenizer::XMLToken::TAG_START)
			{
				director.beginOpeningElement();
				director.transitionTo(director.getInsideElementTagState());
			}
			else if (tokenType == XMLTokenizer::XMLToken::TAG_CLOSE_START)
			{
				director.beginClosingElement();
				director.transitionTo(director.getInsideElementTagState());
			}
		}

		virtual void value(Director & director, Builder & builder, const std::string & text)
		{
			builder.addValue(text);
		}
	};

	class InsideElementTagState : public ParserState
	{
	public:
		virtual void name(Director & director, Builder & builder, const std::string & text)
		{
			if (director.isClosingElement())
				builder.confirmElement(text);
			else
				builder.createElement(text);
		}

		virtual void attribute(Director & director, Builder & builder, const std::string & text)
		{
			builder.createAttribute(text);
			director.transitionToAttribute(director.getInsideElementTagState());
		}

		virtual void end(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType)
		{
			if (tokenType == XMLTokenizer::XMLToken::TAG_END)
			{
				if (director.isClosingElement())
				{
					if (!builder.popElement())
						director.transitionTo(std::shared_ptr<ParserState>());
					else
						director.transitionTo(director.getOutsideTagState());
				}
				else
				{
					builder.pushElement();
					director.transitionTo(director.getOutsideTagState());
				}
			}
			else if (tokenType == XMLTokenizer::XMLToken::NULL_TAG_END)
			{
				director.transitionTo(director.getOutsideTagState());
			}
		}
	};

	class InsideAttributeState : public ParserState
	{
	public:
		virtual void value(Director & director, Builder & builder, const std::string & text)
		{
			builder.valueAttribute(text);
			director.transitionFromAttribute();
		}
	};

}

Director::Director(const std::string & filename, std::shared_ptr<Builder> _builder) :
  builder(_builder),
  attributeReturnState(std::shared_ptr<ParserState>()),
  beforePrologState(new BeforePrologState),
  inPrologState(new InPrologState),
  outsideTagState(new OutsideTagState),
  insideElementTagState(new InsideElementTagState),
  insideAttributeState(new InsideAttributeState),
  closingElement(false)
{
	XMLTokenizer	tokenizer(filename);
	std::shared_ptr<XMLTokenizer::XMLToken>	token;

	transitionTo(beforePrologState);

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

void Director::transitionToAttribute(std::shared_ptr<ParserState> returnState)
{
	attributeReturnState	= returnState;
	transitionTo(insideAttributeState);
}

void Director::transitionFromAttribute(void)
{
	transitionTo(attributeReturnState);
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
