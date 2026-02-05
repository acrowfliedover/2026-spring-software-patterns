package edu.jhu.apl.patterns_class.dom.replacement;

public interface Element extends Node
{
	//
	// Implemented Element members.
	//
	public String getAttribute(String name);
	public Attr getAttributeNode(String name);
	public NodeList getElementsByTagName(String tagName);
	public String getTagName();
	public boolean hasAttribute(String name);
	public void removeAttribute(String name);
	public Attr removeAttributeNode(Attr oldAttr);
	public void setAttribute(String name, String value);
	public Attr setAttributeNode(Attr newAttr);

	//
	// Unimplemented Element members.
	//
	public Attr getAttributeNodeNS(String namespaceURI, String localName);
	public String getAttributeNS(String namespaceURI, String localName);
	public NodeList getElementsByTagNameNS(String tagName);
	public boolean hasAttributeNS(String namespaceURI, String localName);
	public void removeAttributeNS(String namespaceURI, String localName);
	public Attr setAttributeNodeNS(Attr newAttr);
	public void setAttributeNS(String namespaceURI, String localName, String value);
	public Attr setAttributeNS(Attr newAttr);
	public NodeList getElementsByTagNameNS(String namespaceURI, String localName);
	public void setIdAttributeNode(Attr idAttr, boolean isId);
	public void setIdAttributeNS(String namespaceURI, String localName, boolean isId);
	public void setIdAttribute(String name, boolean isId);
	public org.w3c.dom.TypeInfo getSchemaTypeInfo();



	//
	// Reimplemented Node members.
	//
	public NamedNodeMap getAttributes();
	public boolean hasAttributes();
}
