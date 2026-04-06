#include <stdio.h>
#include <iostream>
#include <fstream>
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
#include "StdOutObserver.H"

void testTokenizer(int argc, char** argv);
void testSerializer(int argc, char** argv);
void testValidator(int argc, char** argv);
void testIterator(int argc, char** argv);
void testDirector(int argc, char** argv);
void testChainOfResponsibility(int argc, char** argv);
void testMemento(int argc, char** argv);

void printUsage(void)
{
	printf("Usage:\n");
	printf("\tTest t [file] ...\n");
	printf("\tTest s [file1] [file2]\n");
	printf("\tTest v [file]\n");
	printf("\tTest i\n");
	printf("\tTest d [file1] [file2]\n");
	printf("\tTest c\n");
	printf("\tTest m\n");
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printUsage();
		exit(0);
	}

	switch(argv[1][0])
	{
	case 'T':
	case 't':
		testTokenizer(argc, argv);
		break;
	case 'S':
	case 's':
		testSerializer(argc, argv);
		break;
	case 'V':
	case 'v':
		testValidator(argc, argv);
		break;
	case 'I':
	case 'i':
		testIterator(argc, argv);
		break;
	case 'D':
	case 'd':
		testDirector(argc, argv);
		break;
	case 'C':
	case 'c':
		testChainOfResponsibility(argc, argv);
		break;
	case 'M':
	case 'm':
		testMemento(argc, argv);
		break;
	}
}

void testTokenizer(int argc, char** argv)
{
	std::shared_ptr<dom::Document>	document(std::make_shared<Document_Impl>());

	std::shared_ptr<dom::Element>	element(document->createElement("NewElement"));
	std::shared_ptr<dom::Text>	text(document->createTextNode("Text Data"));
	std::shared_ptr<dom::Attr>	attr(document->createAttribute("NewAttribute"));

	printf("Element Tag = '%s'\n", element->getTagName().c_str());
	printf("Text Data = '%s'\n", text->getValue().c_str());
	printf("Attr Name = '%s'\n", attr->getName().c_str());

	element->setAttributeNode(attr);
	printf("Element attribute '%s'='%s'\n", element->getTagName().c_str(), element->getAttribute("NewAttribute").c_str());

	for (int i = 2; i < argc; i++)
	{
		XMLTokenizer	tokenizer(argv[i]);

		std::shared_ptr<XMLTokenizer::XMLToken>	token;

		printf("File:  '%s'\n", argv[i]);

		do
		{
			token	= tokenizer.getNextToken();

			printf("\tLine %d:  %s = '%s'\n", tokenizer.getLineNumber(),
			  token->toString(), token->getToken().size() == 0 ? "" : token->getToken().c_str());

		} while (token->getTokenType() != XMLTokenizer::XMLToken::NULL_TOKEN);
	}
}

void testSerializer(int argc, char** argv)
{
	if (argc < 4)
	{
		printUsage();
		exit(0);
	}

	//
	// Create tree of this document:
	// <? xml version="1.0" encoding="UTF-8"?>
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
	std::shared_ptr<dom::Document>	document(std::make_shared<Document_Impl>());
	std::shared_ptr<dom::Element>	root(document->createElement("document"));
	document->appendChild(root);

	std::shared_ptr<dom::Element>	child(document->createElement("element"));
	std::shared_ptr<dom::Attr>	attr(document->createAttribute("attribute"));
	attr->setValue("attribute value");
	child->setAttributeNode(attr);
	root->appendChild(child);

	child				= document->createElement("element");
	root->appendChild(child);

	child				= document->createElement("element");
	child->setAttribute("attribute", "attribute value");
	child->setAttribute("attribute2", "attribute2 value");
	std::shared_ptr<dom::Text>	text		= document->createTextNode("Element Value");
	child->appendChild(text);
	root->appendChild(child);

	child				= document->createElement("element");
	root->appendChild(child);

	//
	// Serialize
	//
	std::fstream *	file	= 0;
	XMLSerializer	xmlSerializer(file = new std::fstream(argv[2], std::ios_base::out));
	xmlSerializer.serializePretty(document);
	delete file;
	XMLSerializer	xmlSerializer2(file = new std::fstream(argv[3], std::ios_base::out));
	xmlSerializer2.serializeMinimal(document);
	delete file;

	// delete Document and tree.
}

void testValidator(int argc, char** argv)
{
	if (argc < 3)
	{
		printUsage();
		exit(0);
	}

	//
	// Create tree of this document:
	// <? xml version="1.0" encoding="UTF-8"?>
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

	std::shared_ptr<dom::Document>	document(new DocumentValidator(new Document_Impl, xmlValidator));
	std::shared_ptr<dom::Element>	root;
	std::shared_ptr<dom::Element>	child;
	std::shared_ptr<dom::Attr>	attr;

	root		= document->createElement("document");
	document->appendChild(root);
	child		= document->createElement("element");
	attr		= document->createAttribute("attribute");
	attr->setValue("attribute value");
	child->setAttributeNode(attr);
	root->appendChild(child);
	child		= document->createElement("element");
	root->appendChild(child);
	child		= document->createElement("element");
	child->setAttribute("attribute", "attribute value");
	child->setAttribute("attribute2", "attribute2 value");
	std::shared_ptr<dom::Text>	text(document->createTextNode("Element Value"));
	child->appendChild(text);
	root->appendChild(child);
	child		= document->createElement("element");
	root->appendChild(child);

	//
	// Serialize
	//
	std::fstream *	file	= 0;
	XMLSerializer	xmlSerializer(file = new std::fstream(argv[2], std::ios_base::out));
	xmlSerializer.serializePretty(document);
	delete file;

	// delete Document and tree.
}

void testIterator(int argc, char** argv)
{
	//
	// Create tree of this document:
	// <? xml version="1.0" encoding="UTF-8"?>
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

	child						= document->createElement("element");
	root->appendChild(child);
	printf("  < 0x%08lx > (Second node out of iterator)\n", (unsigned long )child.get());

	child						= document->createElement("element");
	child->setAttribute("attribute", "attribute value");
	child->setAttribute("attribute2", "attribute2 value");
	std::shared_ptr<dom::Text>	text(document->createTextNode("Element Value"));
	child->appendChild(text);
	root->appendChild(child);
	printf("  < 0x%08lx > (Fourth node out of iterator)\n", (unsigned long )child.get());
	printf("    < 0x%08lx > (Third and deepest node out of iterator)\n", (unsigned long )text.get());

	child						= document->createElement("element");
	root->appendChild(child);
	printf("  < 0x%08lx > (Fifth node out of iterator)\n", (unsigned long )child.get());

	printf("\nDepth first iteration:\n");
	std::shared_ptr<dom::Iterator>	domIterator;
	for (domIterator = document->createIterator(0); domIterator->hasNext();)
		printf("node:  0x%08lx\n", (unsigned long )domIterator->next());
}

void testDirector(int argc, char** argv)
{
	std::shared_ptr<dom::Document>	document(new Document_Impl);
	std::shared_ptr<StdOutObserver>	observer(new StdOutObserver);
	std::shared_ptr<Builder>	builder(new Builder(document, observer));
	Director			director(argv[2], builder);
	std::fstream			file(argv[3], std::ios_base::out);
	XMLSerializer			xmlSerializer(&file);

	xmlSerializer.serializePretty(document);
}

void testMemento(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	std::shared_ptr<XMLValidator>	xmlValidator(new XMLValidator);

	std::shared_ptr<ValidChildren>	root(xmlValidator->addSchemaElement(""));
	root->addValidChild("document", false);

	std::shared_ptr<ValidChildren>	doc(xmlValidator->addSchemaElement("document"));
	doc->addValidChild("element", false);

	std::shared_ptr<ValidChildren>	el(xmlValidator->addSchemaElement("element"));
	el->addValidChild("attribute", true);

	std::cout << "--- original schema (originator) ---\n";
	xmlValidator->printSchema();

	XMLValidator::Memento	memento = xmlValidator->createMemento();

	std::cout << "\n--- caretaker stored memento; modifying schema ---\n";
	doc->setCanHaveText(true);
	xmlValidator->addSchemaElement("extra")->addValidChild("junk", false);

	std::cout << "\n--- schema after modification ---\n";
	xmlValidator->printSchema();

	std::cout << "\n--- caretaker restores from memento ---\n";
	xmlValidator->setMemento(memento);

	std::cout << "\n--- schema after restore (should match original) ---\n";
	xmlValidator->printSchema();
}

void testChainOfResponsibility(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	std::shared_ptr<dom::Document>	document(std::make_shared<Document_Impl>());
	std::shared_ptr<dom::Element>	root(document->createElement("handlers"));
	document->appendChild(root);

	std::shared_ptr<dom::Element>	h1(document->createElement("handler"));
	h1->setAttribute("message", "type1");
	root->appendChild(h1);

	std::shared_ptr<dom::Element>	leaf1(document->createElement("handler"));
	leaf1->setAttribute("message", "type2");
	h1->appendChild(leaf1);

	std::shared_ptr<dom::Element>	leaf2(document->createElement("handler"));
	leaf2->setAttribute("message", "type2");
	h1->appendChild(leaf2);

	std::cout << "--- type1 at first leaf (bubbles to parent) ---\n";
	leaf1->handleEvent("type1");

	std::cout << "\n--- type2 at first leaf (handled locally) ---\n";
	leaf1->handleEvent("type2");

	std::cout << "\n--- type3 at first leaf (unhandled) ---\n";
	leaf1->handleEvent("type3");
}
