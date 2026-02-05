//
//  TextImpl.swift
//  Patterns
//
//  Created by Jon Lindberg on 12/30/18.
//  Copyright © 2018 Jon Lindberg. All rights reserved.
//

import Foundation

class TextImpl : NodeImpl, Text
{
	init(document : Document, value : String)
	{
		super.init(document: document, name: "", type: NodeType.TEXT_NODE)
		self.value	= value
	}
	
	public var name			: String	{ get { return nodeName } }
	public var data			: String	{ get { return nodeValue } set { nodeValue = newValue } }
	public var value		: String	{ get { return data } set { data = newValue } }
	public var length		: Int		{ get { return data.count } }

	public func substringData(offset : Int, count : Int)	-> String
	{
		var	counter		= 0
		var	startIndex	= data.startIndex
		var	stopIndex	= data.endIndex
		
		for index in data.indices
		{
			if offset == counter
			{
				startIndex	= index
			}
			else if (offset + count) == counter
			{
				stopIndex	= index
				break
			}
			
			counter	+= 1
		}
		
		return String(data[startIndex..<stopIndex])
	}
	public func appendData(arg : String)
	{
		data.append(arg)
	}
	public func insertData(offset : Int, arg : String)
	{
		data.insert(contentsOf: arg, at: data.index(data.startIndex, offsetBy: offset))
	}
	public func deleteData(offset : Int, count : Int)
	{
		var	counter		= 0
		var	startIndex	= data.startIndex
		var	stopIndex	= data.endIndex
		
		for index in data.indices
		{
			if offset == counter
			{
				startIndex	= index
			}
			else if (offset + count) == counter
			{
				stopIndex	= index
				break
			}
			
			counter	+= 1
		}
		
		data.removeSubrange(startIndex..<stopIndex)
	}
	public func replaceData(offset : Int, count : Int, arg : String)
	{
		deleteData(offset: offset, count: count)
		insertData(offset: offset, arg: arg)
	}
	public func splitText(offset : Int) throws	-> Text
	{
		if offset >= data.count
		{
			throw DOMException.INDEX_SIZE_ERR(reason: "Index larger than Text node's Value.")
		}
		
		var	counter		= 0
		var	startIndex	= data.startIndex
		
		for index in data.indices
		{
			if offset == counter
			{
				startIndex	= index
				break
			}
			
			counter	+= 1
		}
		
		let text = TextImpl(document: document!, value: String(data[startIndex...]))
		data.removeSubrange(data.startIndex..<startIndex)

		if parentNode != nil
		{
			try _ = insertBefore(newChild: text, refChild: nextSibling)
		}

		return text
	}
}
