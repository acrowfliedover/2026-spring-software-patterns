#include "Invoker.H"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Document.H"
#include "Builder.H"
#include "Director.H"
#include "XMLValidator.H"
#include "XMLSerializer.H"
#include "StdOutObserver.H"

void Invoker::run(void)
{
	std::string	line;

	do
	{
		prompt();
		std::getline(std::cin, line);
		invoke(line);
	} while (running);
}

void Invoker::prompt(void)
{
	std::cout << "\nCommands:" << std::endl;
	std::cout << "\tread [file]" << std::endl;
	std::cout << "\twrite [file]" << std::endl;
	std::cout << "\tprint" << std::endl;
	std::cout << "-->  ";
}

void Invoker::invoke(const std::string & input)
{
	std::vector<std::string>	parsedInput;

	split(input, parsedInput);

	if (!parsedInput.empty())
	{
		if (commands.find(parsedInput[0]) != commands.end())
			commands[parsedInput[0]]->Execute(parsedInput.size() > 1 ? parsedInput[1] : "");
		else
			running	= false;
	}
	else
		running	= false;
}

void Invoker::split(const std::string & input, std::vector<std::string> & output)
{
	std::stringstream	ss(input);
	std::string		item;

	while(std::getline(ss, item, ' '))
		output.push_back(item);
}

void ParseCommand::Execute(const std::string & arg)
{
	if (arg.size() == 0)
	{
		std::cout << "No filename provided." << std::endl;
		return;
	}

	std::shared_ptr<dom::Document>	document(new Document_Impl);
	std::shared_ptr<StdOutObserver>	observer(new StdOutObserver);
	std::shared_ptr<Builder>	builder(new Builder(document, observer));
	//
	// Schema for this document:
	// document contains:  element
	// element contains:  element
	// element contains attributes:  attribute, attribute2
	//
	std::shared_ptr<XMLValidator>	xmlValidator(new XMLValidator);
	std::shared_ptr<ValidChildren>	schemaElement(xmlValidator->addSchemaElement(""));
	schemaElement->addValidChild("document", false);
	schemaElement	= xmlValidator->addSchemaElement("document");
	schemaElement->addValidChild("element", false);
	schemaElement	= xmlValidator->addSchemaElement("element");
	schemaElement->addValidChild("element", false);
	schemaElement->addValidChild("attribute", true);
	schemaElement->addValidChild("attribute2", true);
	schemaElement->setCanHaveText(true);

	Director	director(arg, builder);
	state->setDocument(builder->getDocument());
}

void WriteCommand::Execute(const std::string & arg)
{
	if (arg.size() == 0)
	{
		std::cout << "No filename provided." << std::endl;
		return;
	}

	std::fstream	file(arg.c_str(), std::ios_base::out);
	XMLSerializer	xmlSerializer(&file);
	xmlSerializer.serializePretty(state->getDocument());
}

void PrintCommand::Execute(const std::string & arg)
{
	XMLSerializer	xmlSerializer(&std::cout);
	xmlSerializer.serializePretty(state->getDocument());
	std::cout.flush();
}
