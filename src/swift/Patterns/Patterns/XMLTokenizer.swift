//
//  XMLTokenizer.swift
//  Patterns
//
//  Created by Jon Lindberg on 1/1/19.
//  Copyright © 2019 Jon Lindberg. All rights reserved.
//

import Foundation

public class XMLTokenizer
{
	public enum TokenTypes : String
	{
		case NULL_TOKEN		= "null"
		case PROLOG_START	= "<\\?"
		case PROLOG_IDENTIFIER	= "[[:space:]]*xml"
		case ATTRIBUTE_VALUE	= "\"[^\"]*\""
		case PROLOG_END		= "[[:space:]]*\\?>"
		case TAG_START		= "[[:space:]]*<"
		case ELEMENT		= "[[:space:]]*[[:alpha:]]([[:alnum:]_-]|:)*"
		case ATTRIBUTE		= "[[:space:]]+[[:alpha:]]([[:alnum:]_-]|:)*[[:space:]]*="
		case NULL_TAG_END	= "[[:space:]]*/>"
		case TAG_CLOSE_START	= "[[:space:]]*</"
		case VALUE		= "[^<]*"
		case TAG_END		= "[[:space:]]*>"
		case SPACE_TO_EOL	= "[[:space:]]*"
	}
	
	public class XMLToken
	{
		public init(type : TokenTypes, token : String)
		{
			self.typeImpl	= type
			self.tokenImpl	= token
		}
		
		internal var typeImpl	: TokenTypes
		internal var tokenImpl	: String
		
		public var type		: TokenTypes	{ get { return typeImpl } }
		public var token	: String	{ get { return tokenImpl } }
	}
	
	init?(filename : String)
	{
		do
		{
			let data = try String(contentsOf: URL(fileURLWithPath: filename))
			file = data.components(separatedBy: .newlines)
		}
		catch
		{
			print(error)
			return nil
		}
		
		if file.count == 0
		{
			print("Empty file")
			return nil
		}
		
		null_token		= try! NSRegularExpression(pattern: TokenTypes.NULL_TOKEN.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
		prolog_start		= try! NSRegularExpression(pattern: TokenTypes.PROLOG_START.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
		prolog_identifier	= try! NSRegularExpression(pattern: TokenTypes.PROLOG_IDENTIFIER.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
		attribute_value		= try! NSRegularExpression(pattern: TokenTypes.ATTRIBUTE_VALUE.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
		prolog_end		= try! NSRegularExpression(pattern: TokenTypes.PROLOG_END.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
		tag_start		= try! NSRegularExpression(pattern: TokenTypes.TAG_START.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
		element			= try! NSRegularExpression(pattern: TokenTypes.ELEMENT.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
		attribute		= try! NSRegularExpression(pattern: TokenTypes.ATTRIBUTE.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
		null_tag_end		= try! NSRegularExpression(pattern: TokenTypes.NULL_TAG_END.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
		tag_close_start		= try! NSRegularExpression(pattern: TokenTypes.TAG_CLOSE_START.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
		value			= try! NSRegularExpression(pattern: TokenTypes.VALUE.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
		tag_end			= try! NSRegularExpression(pattern: TokenTypes.TAG_END.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
		space_to_eol		= try! NSRegularExpression(pattern: TokenTypes.SPACE_TO_EOL.rawValue, options:  [NSRegularExpression.Options.anchorsMatchLines, NSRegularExpression.Options.useUnixLineSeparators])
	}
	
	internal var file		= [String]()
	internal var line		= ""
	internal var lineNumberImpl	: Int	= 0
	internal var index		: Int	= 0
	internal var insideTag		: Bool	= false

	internal var null_token		: NSRegularExpression
	internal var prolog_start	: NSRegularExpression
	internal var prolog_identifier	: NSRegularExpression
	internal var attribute_value	: NSRegularExpression
	internal var prolog_end		: NSRegularExpression
	internal var tag_start		: NSRegularExpression
	internal var element		: NSRegularExpression
	internal var attribute		: NSRegularExpression
	internal var null_tag_end	: NSRegularExpression
	internal var tag_close_start	: NSRegularExpression
	internal var value		: NSRegularExpression
	internal var tag_end		: NSRegularExpression
	internal var space_to_eol	: NSRegularExpression
	
	public var lineNumber : Int	{ get { return lineNumberImpl } }
	public var lineCharacter : Int	{ get { return index } }
	
	func updateMatchers(type : TokenTypes, newTag : Bool, matcher : NSTextCheckingResult) -> XMLToken
	{
		var	counter		= 0
		var	startIndex	= line.startIndex
		
		for index in line.indices
		{
			if matcher.range.upperBound == counter
			{
				startIndex	= index
				break
			}
			
			counter	+= 1
		}
		
		if counter == line.count
		{
			startIndex	= line.endIndex
		}

		let token	= XMLToken(type: type, token: String(line[..<startIndex]))
		insideTag	= newTag
		line		= String(line[startIndex...])
		index		+= matcher.range.length
		return token
	}
	public func getNextToken() -> XMLToken
	{
		if lineNumber >= file.count
		{
			return XMLToken(type: TokenTypes.NULL_TOKEN, token: "")
		}
		else if line.count == 0
		{
			line	= file[lineNumber]
			index	= 0
			lineNumberImpl	+= 1
			
			if lineNumber >= file.count
			{
				return XMLToken(type: TokenTypes.NULL_TOKEN, token: "")
			}
		}
		else if let result = prolog_start.firstMatch(in: line, options: [], range: NSMakeRange(0, line.count)),
		  result.range.lowerBound == 0
		{
			return updateMatchers(type: TokenTypes.PROLOG_START, newTag: true, matcher: result)
		}
		else if let result = prolog_end.firstMatch(in: line, options: [], range: NSMakeRange(0, line.count)),
		  result.range.lowerBound == 0
		{
			return updateMatchers(type: TokenTypes.PROLOG_END, newTag: false, matcher: result)
		}
		else if let result = tag_close_start.firstMatch(in: line, options: [], range: NSMakeRange(0, line.count)),
			result.range.lowerBound == 0
		{
			return updateMatchers(type: TokenTypes.TAG_CLOSE_START, newTag: true, matcher: result)
		}
		else if let result = tag_start.firstMatch(in: line, options: [], range: NSMakeRange(0, line.count)),
			result.range.lowerBound == 0
		{
			return updateMatchers(type: TokenTypes.TAG_START, newTag: true, matcher: result)
		}
		else if let result = tag_end.firstMatch(in: line, options: [], range: NSMakeRange(0, line.count)),
			result.range.lowerBound == 0
		{
			return updateMatchers(type: TokenTypes.TAG_END, newTag: false, matcher: result)
		}
		else if let result = null_tag_end.firstMatch(in: line, options: [], range: NSMakeRange(0, line.count)),
			result.range.lowerBound == 0
		{
			return updateMatchers(type: TokenTypes.NULL_TAG_END, newTag: false, matcher: result)
		}
		else if insideTag
		{
			if let result = prolog_identifier.firstMatch(in: line, options: [], range: NSMakeRange(0, line.count)),
				result.range.lowerBound == 0
			{
				return updateMatchers(type: TokenTypes.PROLOG_IDENTIFIER, newTag: insideTag, matcher: result)
			}
			else if let result = attribute.firstMatch(in: line, options: [], range: NSMakeRange(0, line.count)),
				result.range.lowerBound == 0
			{
				return updateMatchers(type: TokenTypes.ATTRIBUTE, newTag: insideTag, matcher: result)
			}
			else if let result = attribute_value.firstMatch(in: line, options: [], range: NSMakeRange(0, line.count)),
				result.range.lowerBound == 0
			{
				return updateMatchers(type: TokenTypes.ATTRIBUTE_VALUE, newTag: insideTag, matcher: result)
			}
			else if let result = element.firstMatch(in: line, options: [], range: NSMakeRange(0, line.count)),
				result.range.lowerBound == 0
			{
				return updateMatchers(type: TokenTypes.ELEMENT, newTag: insideTag, matcher: result)
			}
		}

		if let result = value.firstMatch(in: line, options: [], range: NSMakeRange(0, line.count)),
			result.range.lowerBound == 0
		{
			return updateMatchers(type: TokenTypes.VALUE, newTag: insideTag, matcher: result)
		}
		else if let result = space_to_eol.firstMatch(in: line, options: [], range: NSMakeRange(0, line.count)),
			result.range.lowerBound == 0
		{
			return updateMatchers(type: TokenTypes.SPACE_TO_EOL, newTag: insideTag, matcher: result)
		}
		
		return XMLToken(type: TokenTypes.NULL_TOKEN, token: "")
	}
}
