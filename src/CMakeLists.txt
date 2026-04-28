cmake_minimum_required (VERSION 3.23)
project (Patterns)

include_directories(xerces)
add_executable(Patterns Test.C)
set_property(TARGET Patterns PROPERTY CXX_STANDARD 20)
target_sources(Patterns PUBLIC
	Attr.C
	Attr.H
	Builder.C
	Builder.H
	Director.C
	Director.H
	Document.C
	Document.H
	Element.C
	Element.H
	Invoker.C
	Invoker.H
	Mediator.C
	Mediator.H
	NamedNodeMap.C
	NamedNodeMap.H
	Node.C
	Node.H
	NodeList.H
	Observer.H
	ParserState.C
	ParserState.H
	SerializeVisitor.C
	SerializeVisitor.H
	StdOutObserver.C
	StdOutObserver.H
	Subject.C
	Subject.H
	Test.C
	Text.C
	Text.H
	XMLSerializer.C
	XMLSerializer.H
	XMLTokenizer.C
	XMLTokenizer.H
	XMLValidator.C
	XMLValidator.H
	ValidChildren.C
	ValidChildren.H
	Visitor.H
	WhitespaceStrategy.H
	)
