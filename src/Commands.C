#include "Commands.H"

#include <fstream>
#include <iostream>
#include <ios>

#include "Builder.H"
#include "Director.H"
#include "StdOutObserver.H"
#include "XMLSerializer.H"

void	ParseCommand::execute(void)
{
	std::shared_ptr<dom::Document>	fresh(std::make_shared<Document_Impl>());
	std::shared_ptr<StdOutObserver>	observer(std::make_shared<StdOutObserver>());
	std::shared_ptr<Builder>	builder(std::make_shared<Builder>(fresh, observer));
	Director			director(filename, builder);

	session->setDocument(fresh);
	std::cout << "Parsed '" << filename << "'." << std::endl;
}

void	SerializePrettyCommand::execute(void)
{
	std::fstream	file(filename, std::ios_base::out);
	XMLSerializer	xmlSerializer(&file);

	xmlSerializer.serializePretty(session->document());
	std::cout << "Wrote pretty XML to '" << filename << "'." << std::endl;
}

void	SerializeMinimalCommand::execute(void)
{
	std::fstream	file(filename, std::ios_base::out);
	XMLSerializer	xmlSerializer(&file);

	xmlSerializer.serializeMinimal(session->document());
	std::cout << "Wrote minimal XML to '" << filename << "'." << std::endl;
}

void	AddElementCommand::execute(void)
{
	std::shared_ptr<dom::Element>	child(session->document()->createElement(tag));

	parent->appendChild(child);
	std::cout << "Added <" << tag << ">." << std::endl;
}

void	CloneSubtreeCommand::execute(void)
{
	std::shared_ptr<dom::Element>	copy(std::dynamic_pointer_cast<dom::Element>(source->clone()));

	if (!copy)
	{
		std::cout << "Clone failed." << std::endl;
		return;
	}

	destParent->appendChild(copy);
	std::cout << "Cloned <" << source->getTagName() << "> (original=" << source.get()
	  << ", copy=" << copy.get() << ") under <" << destParent->getTagName() << ">." << std::endl;
}

void	ListCommand::execute(void)
{
	std::shared_ptr<dom::Document>		doc(session->document());
	std::shared_ptr<dom::Iterator>		iterator(doc->createIterator(0));

	while (iterator->hasNext())
	{
		dom::Node *	node(iterator->next());
		dom::Element *	element(dynamic_cast<dom::Element *>(node));

		if (element)
			std::cout << "<" << element->getTagName() << ">" << std::endl;
	}
}
