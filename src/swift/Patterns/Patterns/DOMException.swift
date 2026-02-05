//
//  DOMException.swift
//  Patterns
//
//  Created by Jon Lindberg on 12/29/18.
//  Copyright © 2018 Jon Lindberg. All rights reserved.
//

import Foundation

public enum DOMException : Error
{
	case WRONG_DOCUMENT_ERR(reason : String)
	case INUSE_ATTRIBUTE_ERR(reason : String)
	case INDEX_SIZE_ERR(reason : String)
	case NOT_FOUND_ERR(reason : String)
	case NO_MODIFICATION_ALLOWED_ERR(reason : String)
	case INVALID_CHARACTER_ERR(reason : String)
	case HIERARCHY_REQEUST_ERR(reason : String)
}
