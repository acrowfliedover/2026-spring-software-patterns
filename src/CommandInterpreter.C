#include "CommandInterpreter.H"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Commands.H"

void	CommandInterpreter::usage(void)
{
	std::cout
	  << "Commands:\n"
	  << "  parse <file>\n"
	  << "  pretty <file>\n"
	  << "  minimal <file>\n"
	  << "  set <tag> <attr> <value>\n"
	  << "  remove <tag> <attr>\n"
	  << "  add <childTag>             (under document root)\n"
	  << "  add <parentTag> <childTag>\n"
	  << "  clone <srcTag> <dstParentTag>\n"
	  << "  list\n"
	  << "  quit | exit\n";
}

static std::vector<std::string>	tokenize(const std::string & line)
{
	std::vector<std::string>	tokens;
	std::istringstream		stream(line);
	std::string			word;

	while (stream >> word)
		tokens.push_back(word);

	return tokens;
}

void	CommandInterpreter::run(void)
{
	std::string	line;

	usage();
	std::cout << "> " << std::flush;

	while (!stop && std::getline(std::cin, line))
	{
		std::vector<std::string>	tokens(tokenize(line));

		if (tokens.empty())
		{
			std::cout << "> " << std::flush;
			continue;
		}

		const std::string &	verb(tokens[0]);
		std::shared_ptr<Command>	command;

		if (verb == "parse" && tokens.size() == 2)
			command = std::make_shared<ParseCommand>(&session, tokens[1]);
		else if (verb == "pretty" && tokens.size() == 2)
			command = std::make_shared<SerializePrettyCommand>(&session, tokens[1]);
		else if (verb == "minimal" && tokens.size() == 2)
			command = std::make_shared<SerializeMinimalCommand>(&session, tokens[1]);
		else if (verb == "set" && tokens.size() == 4)
		{
			std::shared_ptr<dom::Element>	target(session.findElement(tokens[1]));
			if (target)
				command = std::make_shared<SetAttributeCommand>(target, tokens[2], tokens[3]);
			else
				std::cout << "No element <" << tokens[1] << ">." << std::endl;
		}
		else if (verb == "remove" && tokens.size() == 3)
		{
			std::shared_ptr<dom::Element>	target(session.findElement(tokens[1]));
			if (target)
				command = std::make_shared<RemoveAttributeCommand>(target, tokens[2]);
			else
				std::cout << "No element <" << tokens[1] << ">." << std::endl;
		}
		else if (verb == "add" && tokens.size() == 2)
			command = std::make_shared<AddElementCommand>(&session, session.document(), tokens[1]);
		else if (verb == "add" && tokens.size() == 3)
		{
			std::shared_ptr<dom::Element>	parent(session.findElement(tokens[1]));
			if (parent)
				command = std::make_shared<AddElementCommand>(&session, parent, tokens[2]);
			else
				std::cout << "No element <" << tokens[1] << ">." << std::endl;
		}
		else if (verb == "clone" && tokens.size() == 3)
		{
			std::shared_ptr<dom::Element>	source(session.findElement(tokens[1]));
			std::shared_ptr<dom::Element>	dest(session.findElement(tokens[2]));
			if (source && dest)
				command = std::make_shared<CloneSubtreeCommand>(source, dest);
			else
				std::cout << "Missing source or destination element." << std::endl;
		}
		else if (verb == "list" && tokens.size() == 1)
			command = std::make_shared<ListCommand>(&session);
		else if ((verb == "quit" || verb == "exit") && tokens.size() == 1)
			command = std::make_shared<QuitCommand>(&stop);
		else
			usage();

		invoker.submit(command);

		if (!stop)
			std::cout << "> " << std::flush;
	}
}
