#ifndef PATTERNS_FACADE_H
#define PATTERNS_FACADE_H

#include <string>
#include <vector>

//
// Façade.
//
// Provides a single, simplified interface to the entire patterns
// application.  Each public method hides the construction and wiring
// of the underlying subsystem objects (Document, Builder, Director,
// Tokenizer, Serializer, Validator, Invoker, Observer, ...).
//
class PatternsFacade
{
public:
	PatternsFacade(void)	{}
	~PatternsFacade()	{}

	void	tokenize(const std::vector<std::string> & files);
	void	serialize(const std::string & prettyOut, const std::string & minimalOut);
	void	validate(const std::string & prettyOut);
	void	iterate(void);
	void	buildAndSerialize(const std::string & xmlIn, const std::string & out);
	void	handleEvents(const std::string & xmlIn);
	void	runCommands(void);
	void	prototype(const std::string & out);
	void	interpret(const std::string & xmlIn);
};

#endif // PATTERNS_FACADE_H
