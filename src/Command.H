#ifndef COMMAND_H
#define COMMAND_H

class Command
{
public:
	virtual ~Command() {}
	virtual void	execute(void)	= 0;
};

#endif // COMMAND_H
