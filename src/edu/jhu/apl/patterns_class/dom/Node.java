package edu.jhu.apl.patterns_class.dom;

public class Node implements edu.jhu.apl.patterns_class.dom.replacement.Node
{
	private String		name		= null;
	private String		value		= null;
	private short		nodeType	= -1;
	private Node		parent		= null;
	private NodeList	nodes		= null;
	protected Document	document	= null;

	Node(String name, short type)
	{
		this.name	= name;
		nodeType	= type;
		nodes		= new NodeList();
	}

	void setParent(Node parent)							{ this.parent = parent; }

	//
	// Implemented Interface Members
	//
	public String getNodeName()							{ return name; }
	public String getNodeValue() throws org.w3c.dom.DOMException			{ return value; }
	public void setNodeValue(String nodeValue) throws org.w3c.dom.DOMException	{ value = nodeValue; }
	public short getNodeType()							{ return nodeType; }
	public edu.jhu.apl.patterns_class.dom.replacement.Node getParentNode()		{ return parent; }
	public edu.jhu.apl.patterns_class.dom.replacement.NodeList getChildNodes()	{ return nodes; }
	public edu.jhu.apl.patterns_class.dom.replacement.Node getFirstChild()
	  {return (edu.jhu.apl.patterns_class.dom.replacement.Node)nodes.getFirst();}
	public edu.jhu.apl.patterns_class.dom.replacement.Node getLastChild()
	  {return (edu.jhu.apl.patterns_class.dom.replacement.Node )nodes.getLast();}
	public edu.jhu.apl.patterns_class.dom.replacement.Node getPreviousSibling()
	  { return (edu.jhu.apl.patterns_class.dom.replacement.Node )getSibling(-1);}
	public edu.jhu.apl.patterns_class.dom.replacement.Node getNextSibling()
	  { return (edu.jhu.apl.patterns_class.dom.replacement.Node )getSibling(1); }
	public edu.jhu.apl.patterns_class.dom.replacement.Document getOwnerDocument()	{ return document; }
	public edu.jhu.apl.patterns_class.dom.replacement.Node
	  insertBefore(edu.jhu.apl.patterns_class.dom.replacement.Node newChild,
	  edu.jhu.apl.patterns_class.dom.replacement.Node refChild) throws org.w3c.dom.DOMException
	{
		// TODO:  Do readonly checks on this node and current parent of newChild.  NO_MODIFICATION_ALLOWED_ERR
		// TODO:  Exclude child types not permitted for this element here.  HIERARCHY_REQUEST_ERR

		if (newChild.getOwnerDocument() != getOwnerDocument())
			throw new org.w3c.dom.DOMException(org.w3c.dom.DOMException.WRONG_DOCUMENT_ERR,
			  "New Child is not a part of this document.");

		if (newChild.getParentNode() != null)
			newChild.getParentNode().removeChild(newChild);

		if (refChild == null)
		{
			nodes.addLast(newChild);
			((Node )newChild).setParent(this);
			return newChild;
		}

		int index	= nodes.indexOf(refChild);

		if (index == -1)
			throw new org.w3c.dom.DOMException(org.w3c.dom.DOMException.NOT_FOUND_ERR,
			  "Reference Child is not a child of this node.");

		nodes.add(index, newChild);
		((Node )newChild).setParent(this);

		return newChild;
	}
	public edu.jhu.apl.patterns_class.dom.replacement.Node
	  replaceChild(edu.jhu.apl.patterns_class.dom.replacement.Node newChild,
	  edu.jhu.apl.patterns_class.dom.replacement.Node oldChild) throws org.w3c.dom.DOMException
	{
		// TODO:  Do readonly checks on this node and current parent of newChild.  NO_MODIFICATION_ALLOWED_ERR
		// TODO:  Exclude child types not permitted for this element here.  HIERARCHY_REQUEST_ERR

		if (newChild.getOwnerDocument() != getOwnerDocument())
			throw new org.w3c.dom.DOMException(org.w3c.dom.DOMException.WRONG_DOCUMENT_ERR,
			  "New Child is not a part of this document.");

		if (newChild.getParentNode() != null)
			newChild.getParentNode().removeChild(newChild);

		int index	= nodes.indexOf(oldChild);

		if (index == -1)
			throw new org.w3c.dom.DOMException(org.w3c.dom.DOMException.NOT_FOUND_ERR,
			  "Old Child is not a child of this node.");

		nodes.add(index, newChild);
		((Node )newChild).setParent(this);
		((Node )nodes.get(index + 1)).setParent(null);
		nodes.remove(index + 1);

		return oldChild;
	}
	public edu.jhu.apl.patterns_class.dom.replacement.Node removeChild(edu.jhu.apl.patterns_class.dom.replacement.Node oldChild)
	  throws org.w3c.dom.DOMException
	{
		// TODO:  Do readonly checks on this node.  NO_MODIFICATION_ALLOWED_ERR

		int index	= nodes.indexOf(oldChild);

		if (index == -1)
			throw new org.w3c.dom.DOMException(org.w3c.dom.DOMException.NOT_FOUND_ERR,
			  "Old Child is not a child of this node.");

		((Node )nodes.get(index)).setParent(null);
		nodes.remove(index);

		return oldChild;
	}
	public edu.jhu.apl.patterns_class.dom.replacement.Node appendChild(edu.jhu.apl.patterns_class.dom.replacement.Node newChild)
	  throws org.w3c.dom.DOMException
	{
		// TODO:  Do readonly checks on this node and current parent of newChild.  NO_MODIFICATION_ALLOWED_ERR
		// TODO:  Exclude child types not permitted for this element here.  HIERARCHY_REQUEST_ERR

		if (newChild.getOwnerDocument() != getOwnerDocument())
			throw new org.w3c.dom.DOMException(org.w3c.dom.DOMException.WRONG_DOCUMENT_ERR,
			  "New Child is not a part of this document.");

		if (newChild.getParentNode() != null)
			newChild.getParentNode().removeChild(newChild);

		nodes.addLast(newChild);
		((Node )newChild).setParent(this);

		return newChild;
	}
	public boolean hasChildNodes()					{ return nodes.size() > 0; }
	public String getLocalName()					{ return name; }

	//
	// Unimplemented Interface Members
	//
	public void normalize() {}
	public boolean isSupported(String feature, String version)					{ return false; }
	public String getNamespaceURI()									{ return null; }
	public String getPrefix()									{ return null; }
	public void setPrefix(String prefix) throws org.w3c.dom.DOMException				{}
	public edu.jhu.apl.patterns_class.dom.replacement.Node cloneNode(boolean deep)			{ return null; }
	public boolean hasAttributes()									{ return false; }
	public edu.jhu.apl.patterns_class.dom.replacement.NamedNodeMap getAttributes()			{ return null; }
	public Object getUserData(String key)								{ return null; }
	public Object setUserData(String key, Object data, org.w3c.dom.UserDataHandler handler)		{ return null; }
	public Object getFeature(String feature, String version)					{ return null; }
	public boolean isEqualNode(edu.jhu.apl.patterns_class.dom.replacement.Node arg)			{ return false; }
	public String lookupNamespaceURI(String prefix)							{ return null; }
	public boolean isDefaultNamespace(String namespaceURI)						{ return false; }
	public String lookupPrefix(String namespaceURI)							{ return null; }
	public boolean isSameNode(edu.jhu.apl.patterns_class.dom.replacement.Node other)		{ return false; }
	public void setTextContent(String textContent)							{}
	public String getTextContent()									{ return null; }
	public short compareDocumentPosition(edu.jhu.apl.patterns_class.dom.replacement.Node other)	{ return (short )0; }
	public String getBaseURI()									{ return null; }

	//
	// Class Members
	//
	private Node getSibling(int direction)
	{
		if (parent == null)
			return null;

		java.util.LinkedList	siblings	= (java.util.LinkedList )parent.getChildNodes();

		try
		{
			return (Node )siblings.get(siblings.indexOf(this) + direction);
		}
		catch (java.lang.IndexOutOfBoundsException e)
		{
			return null;
		}
	}
}
