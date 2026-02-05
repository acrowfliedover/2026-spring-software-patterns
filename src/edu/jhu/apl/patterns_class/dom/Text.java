package edu.jhu.apl.patterns_class.dom;

public class Text extends Node implements edu.jhu.apl.patterns_class.dom.replacement.Text
{
	Text(String value, Document document)
	{
		super(null, org.w3c.dom.Node.TEXT_NODE);
		setNodeValue(value);
		this.document	= document;
	}

	//
	// Implemented Text members.
	//
	public String getName()
	{
		return getNodeName();
	}
	public String getData()
	{
		return getNodeValue();
	}
	public String getValue()
	{
		return getData();
	}
	public void setData(String value)
	{
		// TODO:  Check for readonly status.  NO_MODIFICATION_ALLOWED_ERR

		setNodeValue(value);
	}
	public void setValue(String value)
	{
		// TODO:  Check for readonly status.  NO_MODIFICATION_ALLOWED_ERR

		setData(value);
	}
	public int getLength()
	{
		return getValue().length();
	}
	public String substringData(int offset, int count)
	{
		// TODO:  Check for readonly status.  NO_MODIFICATION_ALLOWED_ERR

		try
		{
			return getValue().substring(offset, offset + count);
		}
		catch(java.lang.IndexOutOfBoundsException e)
		{
			throw new org.w3c.dom.DOMException(org.w3c.dom.DOMException.INDEX_SIZE_ERR,
			  "Index " + offset + " larger than Text node's value.");
		}
	}
	public void appendData(String arg)
	{
		// TODO:  Check for readonly status.  NO_MODIFICATION_ALLOWED_ERR

		setValue(getValue() + arg);
	}
	public void insertData(int offset, String arg)
	{
		// TODO:  Check for readonly status.  NO_MODIFICATION_ALLOWED_ERR

		setValue(substringData(0, offset) + arg + substringData(offset, getLength() - offset));
	}
	public void deleteData(int offset, int count)
	{
		// TODO:  Check for readonly status.  NO_MODIFICATION_ALLOWED_ERR

		setValue(substringData(0, offset) + substringData(offset + count, getLength() - (offset + count)));
	}
	public void replaceData(int offset, int count, String arg)
	{
		// TODO:  Check for readonly status.  NO_MODIFICATION_ALLOWED_ERR

		setValue(substringData(0, offset) + arg + substringData(offset + count, getLength() - (offset + count)));
	}
	public edu.jhu.apl.patterns_class.dom.replacement.Text splitText(int offset)
	{
		// TODO:  Check for readonly status.  NO_MODIFICATION_ALLOWED_ERR

		try
		{
			Text	text	= new Text(substringData(offset, getLength() - offset), document);

			if (getParentNode() != null)
			{
				setValue(substringData(0, offset));

				getParentNode().insertBefore(text, getNextSibling());

				return text;
			}
			else
				throw new org.w3c.dom.DOMException(org.w3c.dom.DOMException.NO_MODIFICATION_ALLOWED_ERR,
				  "Unable to split Text node with no parent node.");
		}
		catch(java.lang.IndexOutOfBoundsException e)
		{
			throw new org.w3c.dom.DOMException(org.w3c.dom.DOMException.INDEX_SIZE_ERR,
			  "Index " + offset + " larger than Text node's value.");
		}
	}

	public edu.jhu.apl.patterns_class.dom.replacement.Text replaceWholeText(String content) { return null; }
	public String getWholeText() { return null; }
	public boolean isElementContentWhitespace() { return false; }
}
