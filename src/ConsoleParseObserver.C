#include "ConsoleParseObserver.H"
#include <iostream>

void ConsoleParseObserver::printIndent(int depth) const
{
	for (int i = 0; i < depth; i++)
		std::cout << "  ";
}

void ConsoleParseObserver::update(const ParseEvent & event)
{
	std::cout << "[PARSE] ";
	printIndent(event.depth);

	switch (event.type)
	{
	case ParseEvent::PROLOG_START:
		std::cout << "Prolog started" << std::endl;
		break;
	case ParseEvent::PROLOG_END:
		std::cout << "Prolog ended" << std::endl;
		break;
	case ParseEvent::PROLOG_ID:
		std::cout << "Prolog identifier: " << event.data << std::endl;
		break;
	case ParseEvent::ELEMENT_CREATED:
		std::cout << "Element created: <" << event.data << ">" << std::endl;
		break;
	case ParseEvent::ATTRIBUTE_CREATED:
		std::cout << "Attribute created: " << event.data << std::endl;
		break;
	case ParseEvent::ATTRIBUTE_VALUE:
		std::cout << "Attribute value: " << event.data << std::endl;
		break;
	case ParseEvent::TEXT_ADDED:
		std::cout << "Text added: \"" << event.data << "\"" << std::endl;
		break;
	case ParseEvent::ELEMENT_PUSHED:
		std::cout << "Element pushed (depth: " << event.depth << ")" << std::endl;
		break;
	case ParseEvent::ELEMENT_POPPED:
		std::cout << "Element popped (depth: " << event.depth << ")" << std::endl;
		break;
	case ParseEvent::ELEMENT_CONFIRMED:
		std::cout << "Element confirmed: </" << event.data << ">" << std::endl;
		break;
	}
}
