package edu.jhu.apl.patterns_class.dom;

public class NodeList extends java.util.LinkedList<edu.jhu.apl.patterns_class.dom.replacement.Node>
  implements edu.jhu.apl.patterns_class.dom.replacement.NodeList
{
	public int		getLength()	{ return size(); }
	public edu.jhu.apl.patterns_class.dom.replacement.Node	item(int index)
	  { return (edu.jhu.apl.patterns_class.dom.replacement.Node )get(index); }
}
