package edu.jhu.apl.patterns_class.dom.replacement;

public interface Document extends Node
{
	//
	// Implemented Document members.
	//
	public Element createElement(String tagName) throws org.w3c.dom.DOMException;
	public Text createTextNode(String data);
	public Attr createAttribute(String name) throws org.w3c.dom.DOMException;
	public Element getDocumentElement();

	//
	// Unimplemented Document members.
	//
	public org.w3c.dom.DOMImplementation getImplementation();
	public org.w3c.dom.DocumentType getDoctype();
	public org.w3c.dom.DocumentFragment createDocumentFragment();
	public org.w3c.dom.Comment createComment(String data);
	public org.w3c.dom.CDATASection createCDATASection(String data) throws org.w3c.dom.DOMException;
	public org.w3c.dom.ProcessingInstruction createProcessingInstruction(String target, String data)
	  throws org.w3c.dom.DOMException;
	public org.w3c.dom.EntityReference createEntityReference(String name) throws org.w3c.dom.DOMException;
	public Node importNode(Node importedNode, boolean deep) throws org.w3c.dom.DOMException;
	public Element createElementNS(String namespaceURI, String qualifiedName) throws org.w3c.dom.DOMException;
	public Attr createAttributeNS(String namespaceURI, String qualifiedName) throws org.w3c.dom.DOMException;
	public NodeList getElementsByTagNameNS(String namespaceURI, String localName);
	public Element getElementById(String elementId);
	public Node cloneNode(boolean deep);
	public Node renameNode(Node n, String namespaceURI, String qualifiedName);
	public void normalizeDocument();
	public org.w3c.dom.DOMConfiguration getDomConfig();
	public Node adoptNode(Node source);
	public void setDocumentURI(String documentURI);
	public String getDocumentURI();
	public void setStrictErrorChecking(boolean strictErrorChecking);
	public boolean getStrictErrorChecking();
	public void setXmlVersion(String xmlVersion);
	public String getXmlVersion();
	public void setXmlStandalone(boolean xmlStandalone);
	public boolean getXmlStandalone();
	public String getXmlEncoding();
	public String getInputEncoding();
}
