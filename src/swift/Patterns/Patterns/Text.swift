//
//  Text.swift
//  Patterns
//
//  Created by Jon Lindberg on 12/29/18.
//  Copyright © 2018 Jon Lindberg. All rights reserved.
//

import Foundation

public protocol Text : Node
{
	var name		: String	{ get }
	var data		: String	{ get set }
	var value		: String	{ get set }
	var length		: Int		{ get }

	func substringData(offset : Int, count : Int)	-> String
	func appendData(arg : String)
	func insertData(offset : Int, arg : String)
	func deleteData(offset : Int, count : Int)
	func replaceData(offset : Int, count : Int, arg : String)
	func splitText(offset : Int) throws		-> Text
}
