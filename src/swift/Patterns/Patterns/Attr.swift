//
//  Attr.swift
//  Patterns
//
//  Created by Jon Lindberg on 12/29/18.
//  Copyright © 2018 Jon Lindberg. All rights reserved.
//

import Foundation

public protocol Attr : Node
{
	var name	: String	{ get }
	var value	: String	{ get set }
	var ownerElement: Element?	{ get }
}
