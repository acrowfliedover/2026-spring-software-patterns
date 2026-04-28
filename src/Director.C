#include "Director.H"
#include "Builder.H"

Director::Director(const std::string & filename, std::shared_ptr<Builder> _builder) :
  builder(_builder)
{
	XMLTokenizer	tokenizer(filename);
	std::shared_ptr<XMLTokenizer::XMLToken>	token;

	transitionTo(createBeforePrologState());

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
