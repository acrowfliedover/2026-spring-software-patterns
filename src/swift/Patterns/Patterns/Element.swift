//
//  Element.swift
//  Patterns
//
//  Created by Jon Lindberg on 12/29/18.
//  Copyright © 2018 Jon Lindberg. All rights reserved.
//

import Foundation

public protocol Element : Node
{
	var tagName		: String		{ get }
	var attributes		: [Attr]		{ get }
	var hasAttributes	: Bool			{ get }

	func getAttribute(name : String)		-> String
	func getAttributeNode(name : String)		-> Attr?
	func elementsByTagName(tagName : String)	-> [Node]
	func hasAttribute(name : String)		-> Bool
	func removeAttribute(name : String)
	func removeAttributeNode(oldAttr : Attr) throws	-> Attr
	func setAttribute(name : String, value : String)
	func setAttributeNode(newAttr : Attr) throws	-> Attr?
}
