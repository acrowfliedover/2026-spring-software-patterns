#include "PatternsFacade.H"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <ios>

#include "Attr.H"
#include "Document.H"
#include "Element.H"
#include "Text.H"
#include "XMLTokenizer.H"
#include "XMLSerializer.H"
#include "XMLValidator.H"
#include "Builder.H"
#include "Director.H"
#include "Invoker.H"
#include "StdOutObserver.H"

namespace
{
	//
	// Helper: build the canonical sample document tree used by several
	// of the demos.  Centralised here so the Façade methods stay short.
	//
	// <document>
	//   <element attribute="attribute value"/>
	//   <element/>
	//   <element attribute="attribute value" attribute2="attribute2 value">
	//     Element Value
	//   </element>
	//   <element>
	//   </element>
	// </document>
	//
	void	populateSampleTree(std::shared_ptr<dom::Document> document)
	{
		std::shared_ptr<dom::Element>	root(document->createElement("document"));
		document->appendChild(root);

		std::shared_ptr<dom::Element>	child(document->createElement("element"));
		std::shared_ptr<dom::Attr>	attr(document->createAttribute("attribute"));
		attr->setValue("attribute value");
		child->setAttributeNode(attr);
		root->appendChild(child);

		child	= document->createElement("element");
		root->appendChild(child);

		child	= document->createElement("element");
		child->setAttribute("attribute", "attribute value");
		child->setAttribute("attribute2", "attribute2 value");
		std::shared_ptr<dom::Text>	text(document->createTextNode("Element Value"));
		child->appendChild(text);
		root->appendChild(child);

		child	= document->createElement("element");
		root->appendChild(child);
	}

	//
	// Helper: build the validator/schema used by the validator and
	// prototype demos.
	//
	std::shared_ptr<XMLValidator>	buildSampleValidator(void)
	{
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

		std::shared_ptr<Memento>	m(xmlValidator->CreateMemento());
		xmlValidator->SetMemento(m);

		return xmlValidator;
	}
}

void	PatternsFacade::tokenize(const std::vector<std::string> & files)
{
	std::shared_ptr<dom::Document>	document(std::make_shared<Document_Impl>());

	std::shared_ptr<dom::Element>	element(document->createElement("NewElement"));
	std::shared_ptr<dom::Text>	text(document->createTextNode("Text Data"));
	std::shared_ptr<dom::Attr>	attr(document->createAttribute("NewAttribute"));

	printf("Element Tag = '%s'\n", element->getTagName().c_str());
	printf("Text Data = '%s'\n", text->getValue().c_str());
	printf("Attr Name = '%s'\n", attr->getName().c_str());

	element->setAttributeNode(attr);
	printf("Element attribute '%s'='%s'\n", element->getTagName().c_str(),
	  element->getAttribute("NewAttribute").c_str());

	for (const std::string & file : files)
	{
		XMLTokenizer	tokenizer(file);

		std::shared_ptr<XMLTokenizer::XMLToken>	token;

		printf("File:  '%s'\n", file.c_str());

		do
		{
			token	= tokenizer.getNextToken();

			printf("\tLine %d:  %s = '%s'\n", tokenizer.getLineNumber(),
			  token->toString(),
			  token->getToken().size() == 0 ? "" : token->getToken().c_str());

		} while (token->getTokenType() != XMLTokenizer::XMLToken::NULL_TOKEN);
	}
}

void	PatternsFacade::serialize(const std::string & prettyOut, const std::string & minimalOut)
{
	std::shared_ptr<dom::Document>	document(std::make_shared<Document_Impl>());
	populateSampleTree(document);

	std::fstream *	file	= 0;
	XMLSerializer	xmlSerializer(file = new std::fstream(prettyOut, std::ios_base::out));
	xmlSerializer.serializePretty(document);
	delete file;
	XMLSerializer	xmlSerializer2(file = new std::fstream(minimalOut, std::ios_base::out));
	xmlSerializer2.serializeMinimal(document);
	delete file;
}

void	PatternsFacade::validate(const std::string & prettyOut)
{
	std::shared_ptr<XMLValidator>	xmlValidator(buildSampleValidator());
	std::shared_ptr<dom::Document>	document(new DocumentValidator(new Document_Impl, xmlValidator));
	populateSampleTree(document);

	std::fstream *	file	= 0;
	XMLSerializer	xmlSerializer(file = new std::fstream(prettyOut, std::ios_base::out));
	xmlSerializer.serializePretty(document);
	delete file;
}

void	PatternsFacade::iterate(void)
{
	std::shared_ptr<dom::Document>	document(new Document_Impl);
	std::shared_ptr<dom::Element>	root(document->createElement("document"));
	document->appendChild(root);
	printf("< 0x%08lx > (Last and highest node out of iterator)\n", (unsigned long )root.get());

	std::shared_ptr<dom::Element>	child(document->createElement("element"));
	std::shared_ptr<dom::Attr>	attr(document->createAttribute("attribute"));
	attr->setValue("attribute value");
	child->setAttributeNode(attr);
	root->appendChild(child);
	printf("  < 0x%08lx > (First node out of iterator)\n", (unsigned long )child.get());

	child	= document->createElement("element");
	root->appendChild(child);
	printf("  < 0x%08lx > (Second node out of iterator)\n", (unsigned long )child.get());

	child	= document->createElement("element");
	child->setAttribute("attribute", "attribute value");
	child->setAttribute("attribute2", "attribute2 value");
	std::shared_ptr<dom::Text>	text(document->createTextNode("Element Value"));
	child->appendChild(text);
	root->appendChild(child);
	printf("  < 0x%08lx > (Fourth node out of iterator)\n", (unsigned long )child.get());
	printf("    < 0x%08lx > (Third and deepest node out of iterator)\n", (unsigned long )text.get());

	child	= document->createElement("element");
	root->appendChild(child);
	printf("  < 0x%08lx > (Fifth node out of iterator)\n", (unsigned long )child.get());

	printf("\nDepth first iteration:\n");
	std::shared_ptr<dom::Iterator>	domIterator;
	for (domIterator = document->createIterator(0); domIterator->hasNext();)
		printf("node:  0x%08lx\n", (unsigned long )domIterator->next());
}

void	PatternsFacade::buildAndSerialize(const std::string & xmlIn, const std::string & out)
{
	std::shared_ptr<dom::Document>	document(new Document_Impl);
	std::shared_ptr<StdOutObserver>	observer(new StdOutObserver);
	std::shared_ptr<Builder>	builder(new Builder(document, observer));
	Director			director(xmlIn, builder);
	std::fstream			file(out, std::ios_base::out);
	XMLSerializer			xmlSerializer(&file);

	xmlSerializer.serializePretty(document);
}

void	PatternsFacade::handleEvents(const std::string & xmlIn)
{
	std::shared_ptr<dom::Document>	document(new Document_Impl);
	std::shared_ptr<StdOutObserver>	observer(new StdOutObserver);
	std::shared_ptr<Builder>	builder(new Builder(document, observer));
	Director			director(xmlIn, builder);
	int				typeCounter(1);

	for (std::shared_ptr<dom::Iterator> iterator = document->createIterator(0); iterator->hasNext();)
	{
		dom::Node *	node(iterator->next());

		if (node != 0 && dynamic_cast<dom::Element *>(node) != 0 && !node->hasChildNodes())
		{
			char	tempArray[16];
			snprintf(tempArray, 16, "type%d", typeCounter);
			std::string	tempString(tempArray);
			std::cout << "Sending event type" << typeCounter << " to Element node." << std::endl;
			dynamic_cast<dom::Element *>(node)->HandleRequest(tempString);
			typeCounter++;
		}
	}
}

void	PatternsFacade::runCommands(void)
{
	Invoker	invoker;

	invoker.addCommand("read", new ParseCommand(&invoker));
	invoker.addCommand("write", new WriteCommand(&invoker));
	invoker.addCommand("print", new PrintCommand(&invoker));

	invoker.run();
}

void	PatternsFacade::prototype(const std::string & out)
{
	std::shared_ptr<XMLValidator>	xmlValidator(buildSampleValidator());
	std::shared_ptr<dom::Document>	document(new DocumentValidator(new Document_Impl, xmlValidator));
	populateSampleTree(document);

	std::fstream *	file	= 0;
	XMLSerializer	xmlSerializer(file = new std::fstream(out, std::ios_base::out));
	xmlSerializer.serializePretty(document->getDocumentElement()->cloneNode(true));
	delete file;
}

void	PatternsFacade::interpret(const std::string & xmlIn)
{
	std::shared_ptr<dom::Document>	document(new Document_Impl);
	std::shared_ptr<StdOutObserver>	observer(new StdOutObserver);
	std::shared_ptr<Builder>	builder(new Builder(document, observer));
	Director			director(xmlIn, builder);

	std::shared_ptr<dom::Element>	root(document->getDocumentElement());

	if (!root)
	{
		std::cerr << "No document element." << std::endl;
		return;
	}

	// Interpreter client: the "context" for evaluation is this DOM subtree — built by Director/Builder from xmlIn.
	// Expressions recurse via interpret() return values; no separate Context object.
	const int	result(root->interpret());

	std::cout << result << std::endl;
}
