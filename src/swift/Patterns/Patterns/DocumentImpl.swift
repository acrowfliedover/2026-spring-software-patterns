//
//  DocumentImpl.swift
//  Patterns
//
//  Created by Jon Lindberg on 12/30/18.
//  Copyright © 2018 Jon Lindberg. All rights reserved.
//

import Foundation

class DocumentImpl : NodeImpl, Document
{
	public init()
	{
		super.init(document: nil, name: "", type: NodeType.DOCUMENT_NODE)
		resetDocument(document: self)
	}

	public var documentElement	: Element?
	{
		get
		{
			for n in nodes
			{
				if let element = (n as? Element)
				{
					return element
				}
			}
			
			return nil
		}
	}
	
	public func createElement(tagName : String)	-> Element
	{
		return ElementImpl(document : self, tagName : tagName)
	}
	
	public func createTextNode(data : String)	-> Text
	{
		return TextImpl(document: self, value: data)
	}
	
	public func createAttribute(name : String)	-> Attr
	{
		return AttrImpl(document: self, tagName: name)
	}
}
