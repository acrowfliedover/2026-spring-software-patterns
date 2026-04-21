#ifndef COMMANDINVOKER_H
#define COMMANDINVOKER_H

#include <memory>

#include "Command.H"

class CommandInvoker
{
private:
	std::shared_ptr<Command>	current;
public:
	void	submit(std::shared_ptr<Command> c)
	{
		current	= c;
		if (current)
			current->execute();
	}
};

#endif // COMMANDINVOKER_H
