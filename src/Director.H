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
	virtual void	transitionToAttribute(std::shared_ptr<ParserState> returnState);
	virtual void	transitionFromAttribute(void);
	virtual void	beginOpeningElement(void)	{ closingElement = false; }
	virtual void	beginClosingElement(void)	{ closingElement = true; }
	virtual bool	isClosingElement(void) const	{ return closingElement; }
	virtual std::shared_ptr<ParserState>	getInPrologState(void)		{ return inPrologState; }
	virtual std::shared_ptr<ParserState>	getOutsideTagState(void)		{ return outsideTagState; }
	virtual std::shared_ptr<ParserState>	getInsideElementTagState(void)	{ return insideElementTagState; }
private:
	std::shared_ptr<Builder>		builder;
	std::shared_ptr<ParserState>	state;
	std::shared_ptr<ParserState>	attributeReturnState;
	std::shared_ptr<ParserState>	beforePrologState;
	std::shared_ptr<ParserState>	inPrologState;
	std::shared_ptr<ParserState>	outsideTagState;
	std::shared_ptr<ParserState>	insideElementTagState;
	std::shared_ptr<ParserState>	insideAttributeState;
	bool				closingElement;

	virtual void	dispatch(std::shared_ptr<XMLTokenizer::XMLToken> token);
};

#endif // DIRECTOR_H
