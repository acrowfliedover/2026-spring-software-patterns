package edu.jhu.apl.patterns_class;

public class XMLSerializer
{
	java.io.File		file			= null;
	java.io.BufferedWriter	writer			= null;
	int			indentationLevel	= 0;

	public XMLSerializer(String filename) throws java.io.FileNotFoundException
	{
		file		= new java.io.File(filename);
		writer		= new java.io.BufferedWriter(new java.io.OutputStreamWriter(new java.io.FileOutputStream(file)));
	}

	public void close() throws java.io.IOException
	{
		writer.close();
	}

	private void prettyIndentation() throws java.io.IOException
	{
		for (int i = 0; i < indentationLevel; i++)
			writer.write("\t");
	}

	//
	// Strategize Node data printing.
	// Strategize whitespace insertion.
	// Strategize output stream
	//
	public void serializePretty(edu.jhu.apl.patterns_class.dom.replacement.Node node) throws java.io.IOException
	{
		if (node instanceof edu.jhu.apl.patterns_class.dom.Document)
		{
			writer.write("<? xml version=\"1.0\" encoding=\"UTF-8\"?>");
			writer.write("\n");
			serializePretty(((edu.jhu.apl.patterns_class.dom.replacement.Document )node).getDocumentElement());
		}
		else if (node instanceof edu.jhu.apl.patterns_class.dom.replacement.Element)
		{
			prettyIndentation();
			writer.write("<" + ((edu.jhu.apl.patterns_class.dom.replacement.Element )node).getTagName());

			int	attrCount	= 0;

			for (java.util.ListIterator i =
			  ((edu.jhu.apl.patterns_class.dom.NodeList )node.getAttributes()).listIterator(0);
			  i.hasNext();)
			{
				edu.jhu.apl.patterns_class.dom.replacement.Node	attr =
				  (edu.jhu.apl.patterns_class.dom.replacement.Node )i.next();

				serializePretty(attr);
				attrCount++;
			}

			if (attrCount > 0)
				writer.write(" ");

			if (!((edu.jhu.apl.patterns_class.dom.NodeList )node.getChildNodes()).listIterator(0).hasNext())
			{
				writer.write("/>");
				writer.write("\n");
			}
			else
			{
				writer.write(">");
				writer.write("\n");
				indentationLevel++;

				for (java.util.ListIterator i =
				  ((edu.jhu.apl.patterns_class.dom.NodeList )node.getChildNodes()).listIterator(0);
				  i.hasNext();)
				{
					edu.jhu.apl.patterns_class.dom.replacement.Node	child =
					  (edu.jhu.apl.patterns_class.dom.replacement.Node )i.next();

					if (child instanceof edu.jhu.apl.patterns_class.dom.replacement.Element ||
					  child instanceof edu.jhu.apl.patterns_class.dom.replacement.Text)
						serializePretty(child);
				}

				indentationLevel--;
				prettyIndentation();
				writer.write("</" + ((edu.jhu.apl.patterns_class.dom.replacement.Element )node).getTagName() + ">");
				writer.write("\n");
			}
		}
		else if (node instanceof edu.jhu.apl.patterns_class.dom.replacement.Attr)
		{
			writer.write(" " + ((edu.jhu.apl.patterns_class.dom.replacement.Attr )node).getName() + "=\"" +
			  ((edu.jhu.apl.patterns_class.dom.replacement.Attr )node).getValue() + "\"");
		}
		else if (node instanceof edu.jhu.apl.patterns_class.dom.replacement.Text)
		{
			prettyIndentation();
			writer.write(((edu.jhu.apl.patterns_class.dom.replacement.Text )node).getData());
			writer.write("\n");
		}
	}

	public void serializeMinimal(edu.jhu.apl.patterns_class.dom.replacement.Node node) throws java.io.IOException
	{
		if (node instanceof edu.jhu.apl.patterns_class.dom.Document)
		{
			writer.write("<? xml version=\"1.0\" encoding=\"UTF-8\"?>");
			serializeMinimal(((edu.jhu.apl.patterns_class.dom.replacement.Document )node).getDocumentElement());
		}
		else if (node instanceof edu.jhu.apl.patterns_class.dom.replacement.Element)
		{
			writer.write("<" + ((edu.jhu.apl.patterns_class.dom.replacement.Element )node).getTagName());

			for (java.util.ListIterator i =
			  ((edu.jhu.apl.patterns_class.dom.NodeList )node.getAttributes()).listIterator(0);
			  i.hasNext();)
			{
				edu.jhu.apl.patterns_class.dom.replacement.Node	attr =
				  (edu.jhu.apl.patterns_class.dom.replacement.Node )i.next();

				serializeMinimal(attr);
			}

			if (!((edu.jhu.apl.patterns_class.dom.NodeList )node.getChildNodes()).listIterator(0).hasNext())
				writer.write("/>");
			else
			{
				writer.write(">");

				for (java.util.ListIterator i =
				  ((edu.jhu.apl.patterns_class.dom.NodeList )node.getChildNodes()).listIterator(0);
				  i.hasNext();)
				{
					edu.jhu.apl.patterns_class.dom.replacement.Node	child =
					  (edu.jhu.apl.patterns_class.dom.replacement.Node )i.next();

					if (child instanceof edu.jhu.apl.patterns_class.dom.replacement.Element ||
					  child instanceof edu.jhu.apl.patterns_class.dom.replacement.Text)
						serializeMinimal(child);
				}

				writer.write("</" + ((edu.jhu.apl.patterns_class.dom.replacement.Element )node).getTagName() + ">");
			}
		}
		else if (node instanceof edu.jhu.apl.patterns_class.dom.replacement.Attr)
		{
			writer.write(" " + ((edu.jhu.apl.patterns_class.dom.replacement.Attr )node).getName() + "=\"" +
			  ((edu.jhu.apl.patterns_class.dom.replacement.Attr )node).getValue() + "\"");
		}
		else if (node instanceof edu.jhu.apl.patterns_class.dom.replacement.Text)
		{
			writer.write(((edu.jhu.apl.patterns_class.dom.replacement.Text )node).getData());
		}
	}

	public static void main(String args[])
	{
		if (args.length < 2)
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
		edu.jhu.apl.patterns_class.dom.replacement.Document	document	=
		  new edu.jhu.apl.patterns_class.dom.Document();
		edu.jhu.apl.patterns_class.dom.replacement.Element	root		= document.createElement("document");
		document.appendChild(root);

		edu.jhu.apl.patterns_class.dom.replacement.Element	child		= document.createElement("element");
		edu.jhu.apl.patterns_class.dom.replacement.Attr		attr		= document.createAttribute("attribute");
		attr.setValue("attribute value");
		child.setAttributeNode(attr);
		root.appendChild(child);

		child	= document.createElement("element");
		root.appendChild(child);

		child	= document.createElement("element");
		child.setAttribute("attribute", "attribute value");
		child.setAttribute("attribute2", "attribute2 value");
		edu.jhu.apl.patterns_class.dom.replacement.Text		text		= document.createTextNode("Element Value");
		child.appendChild(text);
		root.appendChild(child);

		child	= document.createElement("element");
		root.appendChild(child);

		//
		// Serialize
		//
		try
		{
			XMLSerializer	xmlSerializer	= new XMLSerializer(args[0]);
			xmlSerializer.serializePretty(document);
			xmlSerializer.close();
			xmlSerializer	= new XMLSerializer(args[1]);
			xmlSerializer.serializeMinimal(document);
			xmlSerializer.close();
		}
		catch (java.io.IOException e)
		{
			System.out.println("Error writing file.");
			e.printStackTrace();
		}
	}
}
