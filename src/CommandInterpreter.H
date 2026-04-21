#ifndef COMMANDINTERPRETER_H
#define COMMANDINTERPRETER_H

#include "CommandInvoker.H"
#include "XMLSession.H"

class CommandInterpreter
{
private:
	XMLSession	session;
	CommandInvoker	invoker;
	bool		stop;

	void	usage(void);
public:
	CommandInterpreter(void) : stop(false) {}

	void	run(void);
};

#endif // COMMANDINTERPRETER_H
