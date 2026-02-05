package edu.jhu.apl.patterns_class.dom.replacement;

public interface NamedNodeMap extends NodeList
{
	public Node getNamedItem(String name);

	public Node setNamedItem(Node arg) throws org.w3c.dom.DOMException;

	public Node removeNamedItem(String name) throws org.w3c.dom.DOMException;

	public Node getNamedItemNS(String namespaceURI, String localName);
	public Node setNamedItemNS(Node arg);
	public Node removeNamedItemNS(String namespaceURI, String localName);
}
