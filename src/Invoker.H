#ifndef INVOKER_H
#define INVOKER_H

#include <string>
#include <vector>
#include <map>
#include "Command.H"

namespace dom
{
	class Document;
};

class Invoker
{
private:
	std::map<std::string, Command *>	commands;
	bool					running;
	std::shared_ptr<dom::Document>		document;

	virtual void				split(const std::string & input, std::vector<std::string> & output);
public:
	Invoker(void) : running(true), document(0)	{}
	virtual ~Invoker()				{}

	virtual void				addCommand(const std::string & name, Command * command)
	  { commands.insert(std::pair<std::string, Command *>(name, command)); }
	virtual void				run(void);
	virtual void				setDocument(std::shared_ptr<dom::Document> d)		{ document = d; }
	virtual std::shared_ptr<dom::Document>	getDocument(void)					{ return document; }
	virtual void				prompt(void);
	virtual void				invoke(const std::string & input);
};

class ParseCommand : public Command
{
private:
	Invoker *	state;
public:
	ParseCommand(Invoker * s) : state(s)	{}
	virtual void	Execute(const std::string & arg);
};

class WriteCommand : public Command
{
private:
	Invoker *	state;
public:
	WriteCommand(Invoker * s) : state(s)	{}
	virtual void	Execute(const std::string & arg);
};

class PrintCommand : public Command
{
private:
	Invoker *	state;
public:
	PrintCommand(Invoker * s) : state(s)	{}
	virtual void	Execute(const std::string & arg);
};

#endif // INVOKER_H
