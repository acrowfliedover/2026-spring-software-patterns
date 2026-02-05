//
//  NodeImpl.swift
//  Patterns
//
//  Created by Jon Lindberg on 12/29/18.
//  Copyright © 2018 Jon Lindberg. All rights reserved.
//

import Foundation

class NodeImpl : Node
{
	init(document : Document?, name : String, type : NodeType)
	{
		self.implName	= name
		self.type	= type
		self.document	= document
		implValue	= ""
		parent		= nil
		nodes		= [Node]()
	}

	internal let implName	: String
	internal var implValue	: String
	internal let type	: NodeType
	internal var parent	: Node?
	internal var nodes	: [Node]
	internal var document	: Document?
	
	func resetDocument(document : Document)
	{
		self.document = document
	}

	public var nodeName		: String	{ get { return implName } }
	public var nodeValue		: String	{ get { return implValue } set { implValue = newValue } }
	public var nodeType		: NodeType	{ get { return type } }
	public var parentNode		: Node?		{ get { return parent } set { parent = newValue } }
	public var childNodes		: [Node]	{ get { return nodes } }
	public var firstChild		: Node?		{ get { if nodes.count > 0 { return nodes[0] } else { return nil } } }
	public var lastChild		: Node?		{ get { if nodes.count > 0 { return nodes[nodes.count - 1] } else { return nil }}}
	public var previousSibling	: Node?		{ get { return getSibling(direction : -1) } }
	public var nextSibling		: Node?		{ get { return getSibling(direction : 1) } }
	public var ownerDocument	: Document	{ get { return document! } }
	public var hasChildNodes	: Bool		{ get { return nodes.count > 0 } }
	public var localName		: String	{ get { return implName } }
	
	public func insertBefore(newChild : Node, refChild : Node?) throws	-> Node
	{
		if newChild.ownerDocument !== ownerDocument
		{
			throw DOMException.WRONG_DOCUMENT_ERR(reason: "New Child is not a part of this document.")
		}
		
		if let oldParent = newChild.parentNode
		{
			_ = try oldParent.removeChild(oldChild : newChild)
		}
		
		if let ref = refChild
		{
			if let index = nodes.firstIndex(where: { $0 === ref } )
			{
				nodes.insert(newChild, at: index + 1)
				(newChild as! NodeImpl).parent = self
			}
			else
			{
				throw DOMException.NOT_FOUND_ERR(reason: "Reference Child is not a chid of this Node.")
			}
		}
		else
		{
			nodes.append(newChild)
			(newChild as! NodeImpl).parent = self
		}

		return newChild
	}
	public func replaceChild(newChild : Node, oldChild : Node) throws	-> Node
	{
		if newChild.ownerDocument !== ownerDocument
		{
			throw DOMException.WRONG_DOCUMENT_ERR(reason: "New Child is not a part of this document.")
		}
		
		if let oldParent = newChild.parentNode
		{
			_ = try oldParent.removeChild(oldChild: newChild)
		}
		
		if let index = nodes.firstIndex(where: { $0 === oldChild } )
		{
			nodes.insert(newChild, at: index)
			nodes.remove(at: index + 1)
			(newChild as! NodeImpl).parent = self
			(oldChild as! NodeImpl).parent = nil
		}
		else
		{
			throw DOMException.NOT_FOUND_ERR(reason: "Old Child is not a child of this Node.")
		}
		
		return oldChild
	}
	public func removeChild(oldChild : Node) throws			-> Node
	{
		if let index = nodes.firstIndex(where: { $0 === oldChild } )
		{
			(nodes[index] as! NodeImpl).parent = nil
			nodes.remove(at: index)
		}
		else
		{
			throw DOMException.NOT_FOUND_ERR(reason: "Old Child is not a child of this Node.")
		}
		
		return oldChild
	}
	public func appendChild(newChild : Node) throws			-> Node
	{
		if newChild.ownerDocument !== ownerDocument
		{
			throw DOMException.WRONG_DOCUMENT_ERR(reason: "New Child is not a part of this document.")
		}
		
		if let oldParent = newChild.parentNode
		{
			_ = try oldParent.removeChild(oldChild: newChild)
		}
		
		nodes.append(newChild)
		(newChild as! NodeImpl).parent = self
		
		return newChild
	}

	internal func getSibling(direction : Int)		-> Node?
	{
		if let thisParent = parent
		{
			if let i = thisParent.childNodes.firstIndex(where: { $0 === self } )
			{
				if direction < 0
				{
					if i == 0
					{
						return nil
					}
					else
					{
						return thisParent.childNodes[i - 1]
					}
				}
				else
				{
					if (i + 1) == thisParent.childNodes.count
					{
						return nil
					}
					else
					{
						return thisParent.childNodes[i + 1]
					}
				}
			}
		}

		return nil
	}
}
