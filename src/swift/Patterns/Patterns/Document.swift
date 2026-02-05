//
//  Document.swift
//  Patterns
//
//  Created by Jon Lindberg on 12/29/18.
//  Copyright © 2018 Jon Lindberg. All rights reserved.
//

import Foundation

public protocol Document : Node
{
	var documentElement	: Element?	{ get }

	func createElement(tagName : String)	-> Element
	func createTextNode(data : String)	-> Text
	func createAttribute(name : String)	-> Attr
}
