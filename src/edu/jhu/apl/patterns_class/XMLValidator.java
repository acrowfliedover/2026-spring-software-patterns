package edu.jhu.apl.patterns_class;

public class XMLValidator
{
	private java.util.Vector<ValidChildren>	schema	= new java.util.Vector<ValidChildren>();

	//
	// Supercedes any existing description for this element.
	//
	public ValidChildren addSchemaElement(String element)
	{
		ValidChildren	schemaElement	= findSchemaElement(element);

		if (schemaElement != null)
			schema.remove(schemaElement);

		schema.add(schemaElement = new ValidChildren(element));
		return schemaElement;
	}

	public ValidChildren findSchemaElement(String element)
	{
		for (int i = 0; i < schema.size(); i++)
			if ((schema.elementAt(i).getThisElement() == null && element == null) ||
			 (schema.elementAt(i).getThisElement()!=null && schema.elementAt(i).getThisElement().compareTo(element)==0))
				return schema.elementAt(i);

		return null;
	}

	public boolean canRootElement(String newElement)
	{
		return canAddElement(null, newElement);
	}

	public boolean canAddElement(edu.jhu.apl.patterns_class.dom.replacement.Element element, String newElement)
	{
		ValidChildren	schemaElement	= findSchemaElement(element == null ? null : element.getTagName());

		return schemaElement == null ? true : schemaElement.childIsValid(newElement, false);
	}

	public boolean canAddText(edu.jhu.apl.patterns_class.dom.replacement.Element element)
	{
		ValidChildren	schemaElement	= findSchemaElement(element.getTagName());

		return schemaElement == null ? true : schemaElement.canHaveText();
	}

	public boolean canAddAttribute(edu.jhu.apl.patterns_class.dom.replacement.Element element, String newAttribute)
	{
		ValidChildren	schemaElement	= findSchemaElement(element.getTagName());

		return schemaElement == null ? true : schemaElement.childIsValid(newAttribute, true);
	}

	//
	// Optional for schema implementation:
	//
	// public static boolean canValue(edu.jhu.apl.patterns_class.dom.replacement.Attribute attribute, String value)
	// {
	// }

	public static void main(String args[])
	{
		if (args.length < 1)
		{
			System.out.println("No output filenames provided.");
			System.exit(0);
		}

		//
		// Create tree of this document:
		// <? xml version="1.0" encoding="UTF-8"?>
		// <document>
		//   <element attribute="attribute value"/>
		//   <element/>
		//   <element attribute="attribute value" attribute2="attribute2 value">
		//     Element Value
		//   </element>
		//   <element>
		//   </element>
		// </document>
		//
		// Schema for this document:
		// document contains:  element
		// element contains:  element
		// element contains attributes:  attribute, attribute2
		//
		XMLValidator	xmlValidator	= new XMLValidator();
		ValidChildren	schemaElement	= xmlValidator.addSchemaElement(null);
		schemaElement.addValidChild("document", false);
		schemaElement	= xmlValidator.addSchemaElement("document");
		schemaElement.addValidChild("element", false);
		schemaElement	= xmlValidator.addSchemaElement("element");
		schemaElement.addValidChild("element", false);
		schemaElement.addValidChild("attribute", true);
		schemaElement.addValidChild("attribute2", true);
		schemaElement.setCanHaveText(true);

		edu.jhu.apl.patterns_class.dom.replacement.Document	document	=
		  new edu.jhu.apl.patterns_class.dom.Document();
		edu.jhu.apl.patterns_class.dom.replacement.Element	root		= null;
		edu.jhu.apl.patterns_class.dom.replacement.Element	child		= null;
		edu.jhu.apl.patterns_class.dom.replacement.Attr		attr		= null;

		if (xmlValidator.canRootElement("document"))
		{
			root	= document.createElement("document");
			document.appendChild(root);
		}
		else
		{
			System.out.println("Attempted invalid schema operation.");
			System.exit(0);
		}

		if (xmlValidator.canAddElement(root, "element"))
		{
			child	= document.createElement("element");

			if (xmlValidator.canAddAttribute(child, "attribute"))
			{
				attr	= document.createAttribute("attribute");
				attr.setValue("attribute value");
				child.setAttributeNode(attr);
			}
			else
			{
				System.out.println("Attempted invalid schema operation.");
				System.exit(0);
			}

			root.appendChild(child);
		}
		else
		{
			System.out.println("Attempted invalid schema operation.");
			System.exit(0);
		}

		if (xmlValidator.canAddElement(root, "element"))
		{
			child	= document.createElement("element");
			root.appendChild(child);
		}
		else
		{
			System.out.println("Attempted invalid schema operation.");
			System.exit(0);
		}

		if (xmlValidator.canAddElement(root, "element"))
		{
			child	= document.createElement("element");

			if (xmlValidator.canAddAttribute(child, "attribute"))
				child.setAttribute("attribute", "attribute value");
			else
			{
				System.out.println("Attempted invalid schema operation.");
				System.exit(0);
			}

			if (xmlValidator.canAddAttribute(child, "attribute2"))
				child.setAttribute("attribute2", "attribute2 value");
			else
			{
				System.out.println("Attempted invalid schema operation.");
				System.exit(0);
			}

			if (xmlValidator.canAddText(child))
			{
				edu.jhu.apl.patterns_class.dom.replacement.Text text = document.createTextNode("Element Value");
				child.appendChild(text);
			}
			else
			{
				System.out.println("Attempted invalid schema operation.");
				System.exit(0);
			}

			root.appendChild(child);
		}
		else
		{
			System.out.println("Attempted invalid schema operation.");
			System.exit(0);
		}

		if (xmlValidator.canAddElement(root, "element"))
		{
			child	= document.createElement("element");
			root.appendChild(child);
		}
		else
		{
			System.out.println("Attempted invalid schema operation.");
			System.exit(0);
		}

		//
		// Serialize
		//
		try
		{
			XMLSerializer	xmlSerializer	= new XMLSerializer(args[0]);
			xmlSerializer.serializePretty(document);
			xmlSerializer.close();
		}
		catch (java.io.IOException e)
		{
			System.out.println("Error writing file.");
			e.printStackTrace();
		}
	}
}

class ValidChildren
{
	private String				thisElement		= null;	// A value of null represents Document.
	private java.util.Vector<String>	validChildren		= new java.util.Vector<String>();
	private java.util.Vector<Boolean>	childIsAttribute	= new java.util.Vector<Boolean>();
	private boolean				_canHaveText		= false;

	public ValidChildren(String thisElement)		{ this.thisElement = thisElement; }

	public String	getThisElement()			{ return thisElement; }
	public boolean	canHaveText()				{ return _canHaveText; }
	public void	setCanHaveText(boolean _canHaveText)	{ this._canHaveText = _canHaveText; }

	public void	addValidChild(String child, boolean isAttribute)
	{
		if (childIsValid(child, isAttribute))
			return;

		validChildren.add(child);
		childIsAttribute.add(new Boolean(isAttribute));
	}

	public boolean	childIsValid(String child, boolean isAttribute)
	{
		for (int i = 0; i < validChildren.size(); i++)
			if (childIsAttribute.elementAt(i).booleanValue() == isAttribute &&
			  validChildren.elementAt(i).compareTo(child) == 0)
				return true;

		return false;
	}
}
