#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <string>
#include <memory>
#include "ParserState.H"
#include "XMLTokenizer.H"

class Builder;

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
