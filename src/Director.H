#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <string>
#include <memory>
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

class Director
{
public:
	Director(const std::string & filename, std::shared_ptr<Builder> builder);

	virtual void	transitionTo(std::shared_ptr<ParserState> nextState);
private:
	std::shared_ptr<Builder>		builder;
	std::shared_ptr<ParserState>	state;

	virtual void	dispatch(std::shared_ptr<XMLTokenizer::XMLToken> token);
};

#endif // DIRECTOR_H
