#ifndef PARSER_STATE_H
#define PARSER_STATE_H

#include <memory>
#include <string>
#include "XMLTokenizer.H"

class Builder;
class Director;

class ParserState
{
public:
	virtual ~ParserState() {}

	virtual void	start(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType) {}
	virtual void	name(Director & director, Builder & builder, const std::string & text) {}
	virtual void	attribute(Director & director, Builder & builder, const std::string & text) {}
	virtual void	value(Director & director, Builder & builder, const std::string & text) {}
	virtual void	end(Director & director, Builder & builder, XMLTokenizer::XMLToken::TokenTypes tokenType) {}
};

// initial state
std::shared_ptr<ParserState>	createBeforePrologState(void);

#endif // PARSER_STATE_H
