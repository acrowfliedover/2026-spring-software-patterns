package edu.jhu.apl.patterns_class.dom.replacement;

public interface Text extends Node
{
	//
	// Implemented Text members.
	//
	public String getName();
	public String getData();
	public String getValue();
	public void setData(String value);
	public void setValue(String value);
	public int getLength();
	public String substringData(int offset, int count);
	public void appendData(String arg);
	public void insertData(int offset, String arg);
	public void deleteData(int offset, int count);
	public void replaceData(int offset, int count, String arg);
	public Text splitText(int offset);

	public Text replaceWholeText(String content);
	public String getWholeText();
	public boolean isElementContentWhitespace();
}
