//
//  Node.swift
//  Patterns
//
//  Created by Jon Lindberg on 12/29/18.
//  Copyright © 2018 Jon Lindberg. All rights reserved.
//

import Foundation

public enum NodeType
{
	case ATTRIBUTE_NODE
	case DOCUMENT_NODE
	case ELEMENT_NODE
	case TEXT_NODE
}

public protocol Node : class
{
	var nodeName		: String	{ get }
	var nodeValue		: String	{ get set }
	var nodeType		: NodeType	{ get }
	var parentNode		: Node?		{ get }
	var childNodes		: [Node]	{ get }
	var firstChild		: Node?		{ get }
	var lastChild		: Node?		{ get }
	var previousSibling	: Node?		{ get }
	var nextSibling		: Node?		{ get }
	var ownerDocument	: Document	{ get }
	var hasChildNodes	: Bool		{ get }
	var localName		: String	{ get }

	func insertBefore(newChild : Node, refChild : Node?) throws	-> Node
	func replaceChild(newChild : Node, oldChild : Node) throws	-> Node
	func removeChild(oldChild : Node) throws			-> Node
	func appendChild(newChild : Node) throws			-> Node
}
