//
//  AttrImpl.swift
//  Patterns
//
//  Created by Jon Lindberg on 12/30/18.
//  Copyright © 2018 Jon Lindberg. All rights reserved.
//

import Foundation

class AttrImpl : NodeImpl, Attr
{
	init(document : Document, tagName : String)
	{
		super.init(document: document, name: tagName, type: NodeType.ATTRIBUTE_NODE)
	}
	
	public var name	: String		{ get { return nodeName } }
	public var value	: String	{ get { return nodeValue } set { nodeValue = newValue } }
	public var ownerElement : Element?	{ get { return parentNode as? Element } }
}
