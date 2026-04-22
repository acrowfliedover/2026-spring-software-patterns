#ifndef WHITESPACESTRATEGY_H
#define WHITESPACESTRATEGY_H

#include <iostream>

//
// Whitespace Strategy
//
class WhitespaceStrategy
{
public:
	virtual void prettyIndentation(std::ostream * wwriter)	= 0;
	virtual void incrementIndentation(void)			= 0;
	virtual void decrementIndentation(void)			= 0;
	virtual void newLine(std::ostream * wwriter)		= 0;
	virtual ~WhitespaceStrategy() {}
};

#endif // WHITESPACESTRATEGY_H
