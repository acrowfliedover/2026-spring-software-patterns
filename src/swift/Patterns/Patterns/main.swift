//
//  main.swift
//  Patterns
//
//  Created by Jon Lindberg on 12/29/18.
//  Copyright © 2018 Jon Lindberg. All rights reserved.
//

import Foundation

func printUsage()
{
	print("Usage:")
	print("\tTest t [file] ...")
	print("\tTest s [file1] [file2]")
	print("\tTest v [file]")
}


func testTokenizer()
{
	let document	= DocumentImpl()
	
	let element	= document.createElement(tagName: "NewElement")
	let text	= document.createTextNode(data: "Text Data")
	let attr	= document.createAttribute(name: "NewAttribute")
	
	print("Element Tag = '\(element.tagName)'")
	print("Text Data = '\(text.value)'")
	print("Attr Name = '\(attr.name)'")
	
	_ = try! element.setAttributeNode(newAttr: attr)
	let attrValue = element.getAttribute(name: "NewAttribute")
	print("Element attribute '\(attr.name)'='\(attrValue)'")
	
	for i in 2..<CommandLine.argc
	{
		print("Tokenizing file '\(CommandLine.arguments[Int(i)])'")
		
		if let tokenizer = XMLTokenizer(filename: CommandLine.arguments[Int(i)])
		{
			var token = XMLTokenizer.XMLToken(type: XMLTokenizer.TokenTypes.NULL_TOKEN, token: "")
			
			repeat
			{
				token	= tokenizer.getNextToken()
			
				print("\tLine \(tokenizer.lineNumber):  \(token.type) = '\(token.token)'")
			} while token.type != XMLTokenizer.TokenTypes.NULL_TOKEN
		}
		else
		{
			print("Unable to open file.")
		}
	}
}

func testSerializer()
{
	if CommandLine.argc < 4
	{
		printUsage()
		exit(0)
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
	let document	= DocumentImpl()
	let root	= document.createElement(tagName: "document")
	_ = try! document.appendChild(newChild: root)
	
	var child	= document.createElement(tagName: "element")
	let attr	= document.createAttribute(name: "attribute")
	attr.value	= "attribute value"
	_ = try! child.setAttributeNode(newAttr: attr)
	_ = try! root.appendChild(newChild: child)
	
	child		= document.createElement(tagName: "element")
	_ = try! root.appendChild(newChild: child)
	
	child		= document.createElement(tagName: "element")
	child.setAttribute(name: "attribute", value: "attribute value")
	child.setAttribute(name: "attribute2", value: "attribute2 value")
	let text	= document.createTextNode(data: "Element Value")
	_ = try! child.appendChild(newChild: text)
	_ = try! root.appendChild(newChild: child)
	
	//
	// Serialize
	//
	if let xmlSerializer = XMLSerializer(filename: CommandLine.arguments[2])
	{
		xmlSerializer.serializePretty(node: document)
	}
	if let xmlSerializer = XMLSerializer(filename: CommandLine.arguments[3])
	{
		xmlSerializer.serializeMinimal(node: document)
	}
}

func testValidator()
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
	// Schema for this document:
	// document contains:  element
	// element contains:  element
	// element contains attributes:  attribute, attribute2
	//
	let xmlValidator		= XMLValidator()
	var schemaElement		= xmlValidator.addSchemaElement(element: "")
	schemaElement.addValidChild(child: "document", isAttribute: false)
	schemaElement			= xmlValidator.addSchemaElement(element: "document")
	schemaElement.addValidChild(child: "element", isAttribute: false)
	schemaElement			= xmlValidator.addSchemaElement(element: "element")
	schemaElement.addValidChild(child: "element", isAttribute: false)
	schemaElement.addValidChild(child: "attribute", isAttribute: true)
	schemaElement.addValidChild(child: "attribute2", isAttribute: true)
	schemaElement.canHaveText	= true
	
	let document	= DocumentImpl()
	var root	: Element
	var child	: Element
	var attr	: Attr
	
	if xmlValidator.canRootElement(newElement: "document")
	{
		root	= document.createElement(tagName: "document")
		_ = try! document.appendChild(newChild: root)
	}
	else
	{
		print("Attempted invalid schema operation.")
		exit(0)
	}
	
	if xmlValidator.canAddElement(element: root, newElement: "element")
	{
		child	= document.createElement(tagName: "element")
		
		if xmlValidator.canAddAttribute(element: child, newAttribute: "attribute")
		{
			attr		= document.createAttribute(name: "attribute")
			attr.value	= "attribute value"
			_ = try! child.setAttributeNode(newAttr: attr)
		}
		else
		{
			print("Attempted invalid schema operation.")
			exit(0)
		}
		
		_ = try! root.appendChild(newChild: child)
	}
	
	if xmlValidator.canAddElement(element: root, newElement: "element")
	{
		child	= document.createElement(tagName: "element")
		_ = try! root.appendChild(newChild: child)
	}
	else
	{
		print("Attempted invalid schema operation.")
		exit(0)
	}
	
	if xmlValidator.canAddElement(element: root, newElement: "element")
	{
		child	= document.createElement(tagName: "element")
		
		if xmlValidator.canAddAttribute(element: child, newAttribute: "attribute")
		{
			child.setAttribute(name: "attribute", value: "attribute value")
		}
		else
		{
			print("Attempted invalid schema operation.")
			exit(0)
		}
		
		if xmlValidator.canAddAttribute(element: child, newAttribute: "attribute2")
		{
			child.setAttribute(name: "attribute2", value: "attribute2 value")
		}
		else
		{
			print("Attempted invalid schema operation.")
			exit(0)
		}
		
		if xmlValidator.canAddText(element: child)
		{
			let text	= document.createTextNode(data: "Element Value")
			_ = try! child.appendChild(newChild: text)
		}
		else
		{
			print("Attempted invalid schema operation.")
			exit(0)
		}
		
		_ = try! root.appendChild(newChild: child)
	}
	else
	{
		print("Attempted invalid schema operation.")
		exit(0)
	}
	
	if xmlValidator.canAddElement(element: root, newElement: "element")
	{
		child	= document.createElement(tagName: "element")
		_ = try! root.appendChild(newChild: child)
	}
	else
	{
		print("Attempted invalid schema operation.")
		exit(0)
	}
	
	//
	// Serialize
	//
	if let xmlSerializer = XMLSerializer(filename: CommandLine.arguments[2])
	{
		xmlSerializer.serializePretty(node: document)
	}
}

if CommandLine.argc < 3
{
	printUsage()
	exit(0)
}
	
switch CommandLine.arguments[1].prefix(1)
{
case "T", "t":
	testTokenizer()
case "S", "s":
	testSerializer()
case "V", "v":
	testValidator()
default:
	printUsage()
}
