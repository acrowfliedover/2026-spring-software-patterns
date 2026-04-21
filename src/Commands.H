#ifndef COMMANDS_H
#define COMMANDS_H

#include <memory>
#include <string>

#include "Command.H"
#include "Document.H"
#include "Element.H"
#include "XMLSession.H"

class ParseCommand : public Command
{
private:
	XMLSession *	session;
	std::string	filename;
public:
	ParseCommand(XMLSession * s, const std::string & f) : session(s), filename(f) {}
	virtual void	execute(void);
};

class SerializePrettyCommand : public Command
{
private:
	XMLSession *	session;
	std::string	filename;
public:
	SerializePrettyCommand(XMLSession * s, const std::string & f) : session(s), filename(f) {}
	virtual void	execute(void);
};

class SerializeMinimalCommand : public Command
{
private:
	XMLSession *	session;
	std::string	filename;
public:
	SerializeMinimalCommand(XMLSession * s, const std::string & f) : session(s), filename(f) {}
	virtual void	execute(void);
};

class SetAttributeCommand : public Command
{
private:
	std::shared_ptr<dom::Element>	target;
	std::string			name;
	std::string			value;
public:
	SetAttributeCommand(std::shared_ptr<dom::Element> t, const std::string & n, const std::string & v) :
	  target(t), name(n), value(v) {}
	virtual void	execute(void)	{ target->setAttribute(name, value); }
};

class RemoveAttributeCommand : public Command
{
private:
	std::shared_ptr<dom::Element>	target;
	std::string			name;
public:
	RemoveAttributeCommand(std::shared_ptr<dom::Element> t, const std::string & n) : target(t), name(n) {}
	virtual void	execute(void)	{ target->removeAttribute(name); }
};

class AddElementCommand : public Command
{
private:
	XMLSession *			session;
	std::shared_ptr<dom::Node>	parent;
	std::string			tag;
public:
	AddElementCommand(XMLSession * s, std::shared_ptr<dom::Node> p, const std::string & t) :
	  session(s), parent(p), tag(t) {}
	virtual void	execute(void);
};

class CloneSubtreeCommand : public Command
{
private:
	std::shared_ptr<dom::Element>	source;
	std::shared_ptr<dom::Element>	destParent;
public:
	CloneSubtreeCommand(std::shared_ptr<dom::Element> s, std::shared_ptr<dom::Element> d) :
	  source(s), destParent(d) {}
	virtual void	execute(void);
};

class ListCommand : public Command
{
private:
	XMLSession *	session;
public:
	ListCommand(XMLSession * s) : session(s) {}
	virtual void	execute(void);
};

class QuitCommand : public Command
{
private:
	bool *	stopFlag;
public:
	QuitCommand(bool * f) : stopFlag(f) {}
	virtual void	execute(void)	{ *stopFlag = true; }
};

#endif // COMMANDS_H
