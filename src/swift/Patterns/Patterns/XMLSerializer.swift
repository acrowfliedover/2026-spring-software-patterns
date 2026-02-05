//
//  XMLSerializer.swift
//  Patterns
//
//  Created by Jon Lindberg on 12/31/18.
//  Copyright © 2018 Jon Lindberg. All rights reserved.
//

import Foundation

public class XMLSerializer
{
	init?(filename : String)
	{
		//
		// There must be a way to create the file and get a handle to writing to it in one call without
		// jumping down to direct 'C' Posix calls, but I haven't found it yet.
		//
		if !FileManager.default.createFile(atPath: filename, contents: nil, attributes: nil)
		{
			print("Unable to create file '\(filename)'.")
			return nil
		}
		
		do
		{
			let tempFile = try FileHandle(forWritingTo: URL(fileURLWithPath: filename))
			file	= tempFile
		}
		catch
		{
			print(error)
			return nil
		}
	}
	
	internal var indentationLevel	: Int	= 0
	internal var file		: FileHandle
	
	func prettyIndentation()
	{
		if let data = "\t".data(using: String.Encoding.utf8)
		{
			for _ in 0..<indentationLevel
			{
				file.write(data)
			}
		}
	}
	public func serializePretty(node : Node)
	{
		if let doc = node as? Document
		{
			if let header = """
			  <? xml version="1.0" encoding="UTF-8"?>
			  """.data(using: String.Encoding.utf8)
			{
				file.write(header)
				
				if let documentElement = doc.documentElement
				{
					serializePretty(node: documentElement)
				}
			}
		}
		else if let elem = node as? Element
		{
			prettyIndentation()
			if let openTag = ("<" + elem.tagName).data(using: String.Encoding.utf8)
			{
				file.write(openTag)
			}
			
			var attrCount	= 0
			
			for attr in elem.attributes
			{
				serializePretty(node: attr)
				attrCount += 1
			}
			
			if attrCount > 0
			{
				if let space = " ".data(using: String.Encoding.utf8)
				{
					file.write(space)
				}
			}
			
			if elem.childNodes.count == 0
			{
				if let closer = "/>\n".data(using: String.Encoding.utf8)
				{
					file.write(closer)
				}
			}
			else
			{
				if let closer = ">\n".data(using: String.Encoding.utf8)
				{
					file.write(closer)
				}
				
				indentationLevel += 1
					
				for child in elem.childNodes
				{
					if (child as? Element) != nil || (child as? Text) != nil
					{
						serializePretty(node: child)
					}
				}

				indentationLevel -= 1
				prettyIndentation()
				
				if let endTag = ("</" + elem.tagName + ">\n").data(using: String.Encoding.utf8)
				{
					file.write(endTag)
				}
			}
		}
		else if let attr = node as? Attr
		{
			if let attrValue = (" " + attr.name + "=\"" + attr.value + "\"").data(using: String.Encoding.utf8)
			{
				file.write(attrValue)
			}
		}
		else if let text = node as? Text
		{
			prettyIndentation()
			if let data = (text.data + "\n").data(using: String.Encoding.utf8)
			{
				file.write(data)
			}
		}
	}
	public func serializeMinimal(node : Node)
	{
		if let doc = node as? Document
		{
			if let header = "<? xml version=\"1.0\" encoding=\"UTF-8\"?>".data(using: String.Encoding.utf8)
			{
				file.write(header)
				
				if let documentElement = doc.documentElement
				{
					serializeMinimal(node: documentElement)
				}
			}
		}
		else if let elem = node as? Element
		{
			if let openTag = ("<" + elem.tagName).data(using: String.Encoding.utf8)
			{
				file.write(openTag)
			}
			
			for attr in elem.attributes
			{
				serializeMinimal(node: attr)
			}
			
			if elem.childNodes.count == 0
			{
				if let closer = "/>".data(using: String.Encoding.utf8)
				{
					file.write(closer)
				}
			}
			else
			{
				if let closer = ">".data(using: String.Encoding.utf8)
				{
					file.write(closer)
				}
				
				for child in elem.childNodes
				{
					if (child as? Element) != nil || (child as? Text) != nil
					{
						serializeMinimal(node: child)
					}
				}
				
				if let endTag = ("</" + elem.tagName + ">").data(using: String.Encoding.utf8)
				{
					file.write(endTag)
				}
			}
		}
		else if let attr = node as? Attr
		{
			if let attrValue = (" " + attr.name + "=\"" + attr.value + "\"").data(using: String.Encoding.utf8)
			{
				file.write(attrValue)
			}
		}
		else if let text = node as? Text
		{
			if let data = text.data.data(using: String.Encoding.utf8)
			{
				file.write(data)
			}
		}
	}
}
