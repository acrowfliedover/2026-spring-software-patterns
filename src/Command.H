#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command
{
public:
	virtual void	Execute(const std::string & arg)	= 0;
};

#endif // COMMAND_H
