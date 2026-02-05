package edu.jhu.apl.patterns_class.dom.replacement;

public interface Node
{
	//
	// Implemented Interface Members
	//
	public String	getNodeName();
	public String	getNodeValue() throws org.w3c.dom.DOMException;
	public void	setNodeValue(String nodeValue) throws org.w3c.dom.DOMException;
	public short	getNodeType();
	public Node	getParentNode();
	public NodeList	getChildNodes();
	public Node	getFirstChild();
	public Node	getLastChild();
	public Node	getPreviousSibling();
	public Node	getNextSibling();
	public Document	getOwnerDocument();
	public Node	insertBefore(Node newChild, Node refChild) throws org.w3c.dom.DOMException;
	public Node	replaceChild(Node newChild, Node oldChild) throws org.w3c.dom.DOMException;
	public Node	removeChild(Node oldChild) throws org.w3c.dom.DOMException;
	public Node	appendChild(Node newChild) throws org.w3c.dom.DOMException;
	public boolean	hasChildNodes();
	public String	getLocalName();

	//
	// Unimplemented Interface Members
	//
	public void normalize();
	public boolean isSupported(String feature, String version);
	public String getNamespaceURI();
	public String getPrefix();
	public void setPrefix(String prefix) throws org.w3c.dom.DOMException;
	public Node cloneNode(boolean deep);
	public boolean hasAttributes();
	public NamedNodeMap getAttributes();
	public Object getUserData(String key);
	public Object setUserData(String key, Object data, org.w3c.dom.UserDataHandler handler);
	public Object getFeature(String feature, String version);
	public boolean isEqualNode(Node arg);
	public String lookupNamespaceURI(String prefix);
	public boolean isDefaultNamespace(String namespaceURI);
	public String lookupPrefix(String namespaceURI);
	public boolean isSameNode(Node other);
	public void setTextContent(String textContent);
	public String getTextContent();
	public short compareDocumentPosition(Node other);
	public String getBaseURI();
}
