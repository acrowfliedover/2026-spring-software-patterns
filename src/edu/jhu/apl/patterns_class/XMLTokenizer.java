package edu.jhu.apl.patterns_class;

public class XMLTokenizer
{
	private java.io.BufferedReader	reader		= null;
	private String			line		= null;
	private int			line_number	= 0;
	private int			index		= 0;
	private boolean			inside_tag	= false;

	private static final java.util.regex.Pattern	PROLOG_START		= java.util.regex.Pattern.compile("<\\?");
	private static final java.util.regex.Pattern	PROLOG_IDENTIFIER	= java.util.regex.Pattern.compile("\\p{Space}*xml");
	private static final java.util.regex.Pattern	ATTRIBUTE_VALUE		= java.util.regex.Pattern.compile("\"[^\"]*\"");
	private static final java.util.regex.Pattern	PROLOG_END		=java.util.regex.Pattern.compile("\\p{Space}*\\?>");
	private static final java.util.regex.Pattern	TAG_START		= java.util.regex.Pattern.compile("\\p{Space}*<");
	private static final java.util.regex.Pattern	ELEMENT			=
	  java.util.regex.Pattern.compile("\\p{Space}*\\p{Alpha}([\\p{Alnum}_-]|:)*");
	private static final java.util.regex.Pattern	ATTRIBUTE		=
	  java.util.regex.Pattern.compile("\\p{Space}+\\p{Alpha}([\\p{Alnum}_-]|:)*\\p{Space}*=");
	private static final java.util.regex.Pattern	NULL_TAG_END		= java.util.regex.Pattern.compile("\\p{Space}*/>");
	private static final java.util.regex.Pattern	TAG_CLOSE_START		= java.util.regex.Pattern.compile("\\p{Space}*</");
	private static final java.util.regex.Pattern	VALUE			= java.util.regex.Pattern.compile("[^<]*");
	private static final java.util.regex.Pattern	TAG_END			= java.util.regex.Pattern.compile("\\p{Space}*>");
	private static final java.util.regex.Pattern	SPACE_TO_EOL		= java.util.regex.Pattern.compile("\\p{Space}*");

	private java.util.regex.Matcher	prolog_start_matcher;
	private java.util.regex.Matcher	prolog_identifier_matcher;
	private java.util.regex.Matcher	attribute_value_matcher;
	private java.util.regex.Matcher	prolog_end_matcher;
	private java.util.regex.Matcher	tag_start_matcher;
	private java.util.regex.Matcher	element_matcher;
	private java.util.regex.Matcher	attribute_matcher;
	private java.util.regex.Matcher	null_tag_end_matcher;
	private java.util.regex.Matcher	tag_close_start_matcher;
	private java.util.regex.Matcher	value_matcher;
	private java.util.regex.Matcher	tag_end_matcher;
	private java.util.regex.Matcher	space_to_eol_matcher;

	public class XMLToken
	{
		public static final int	NULL			=  0;
		public static final int	PROLOG_START		=  1;
		public static final int	PROLOG_IDENTIFIER	=  2;
		public static final int	ATTRIBUTE_VALUE		=  3;
		public static final int	PROLOG_END		=  4;
		public static final int	TAG_START		=  5;
		public static final int	ELEMENT			=  6;
		public static final int	ATTRIBUTE		=  7;
		public static final int	NULL_TAG_END		=  8;
		public static final int	TAG_CLOSE_START		=  9;
		public static final int	VALUE			= 10;
		public static final int	TAG_END			= 11;

		private String	token		= null;
		private int	token_type	= NULL;

		XMLToken(String t, int tt)
		{
			token		= t;
			token_type	= tt;
		}

		public String	getToken()
		{
			return token;
		}

		public int	getTokenType()
		{
			return token_type;
		}

		public String toString()
		{
			switch(token_type)
			{
			case NULL:
				return "NULL";
			case PROLOG_START:
				return "PROLOG_START";
			case PROLOG_IDENTIFIER:
				return "PROLOG_IDENTIFIER";
			case ATTRIBUTE_VALUE:
				return "ATTRIBUTE_VALUE";
			case PROLOG_END:
				return "PROLOG_END";
			case TAG_START:
				return "TAG_START";
			case ELEMENT:
				return "ELEMENT";
			case ATTRIBUTE:
				return "ATTRIBUTE";
			case NULL_TAG_END:
				return "NULL_TAG_END";
			case TAG_CLOSE_START:
				return "TAG_CLOSE_START";
			case VALUE:
				return "VALUE";
			case TAG_END:
				return "TAG_END";
			default:
				return "UNKNOWN_TOKEN";
			}
		}
	}

	public XMLTokenizer(String filename) throws java.io.FileNotFoundException
	{
		reader	= new java.io.BufferedReader(new java.io.FileReader(filename));
	}

	public int getLineNumber()
	{
		return line_number;
	}

	public int getLineCharacter()
	{
		return index;
	}

	public XMLToken getNextToken() throws java.io.IOException
	{
		if (line == null)
		{
			line	= reader.readLine();
			index	= 0;
			line_number++;

			if (line == null)
				return new XMLToken(null, XMLToken.NULL);

			if (prolog_start_matcher == null)
			{
				prolog_start_matcher		= PROLOG_START.matcher(line);
				prolog_identifier_matcher	= PROLOG_IDENTIFIER.matcher(line);
				attribute_value_matcher		= ATTRIBUTE_VALUE.matcher(line);
				prolog_end_matcher		= PROLOG_END.matcher(line);
				tag_start_matcher		= TAG_START.matcher(line);
				element_matcher			= ELEMENT.matcher(line);
				attribute_matcher		= ATTRIBUTE.matcher(line);
				null_tag_end_matcher		= NULL_TAG_END.matcher(line);
				tag_close_start_matcher		= TAG_CLOSE_START.matcher(line);
				value_matcher			= VALUE.matcher(line);
				tag_end_matcher			= TAG_END.matcher(line);
				space_to_eol_matcher		= SPACE_TO_EOL.matcher(line);
			}
			else
				update_matchers(0);
		}

		if (prolog_start_matcher.lookingAt() && prolog_start_matcher.start() == 0)
		{
			XMLToken	token	= new XMLToken(prolog_start_matcher.group(), XMLToken.PROLOG_START);
			inside_tag		= true;
			update_matchers(prolog_start_matcher.end());
			return token;
		}

		if (prolog_end_matcher.lookingAt() && prolog_end_matcher.start() == 0)
		{
			XMLToken	token	= new XMLToken(prolog_end_matcher.group(), XMLToken.PROLOG_END);
			inside_tag		= false;
			update_matchers(prolog_end_matcher.end());
			return token;
		}

		if (tag_close_start_matcher.lookingAt() && tag_close_start_matcher.start() == 0)
		{
			XMLToken	token	= new XMLToken(tag_close_start_matcher.group(), XMLToken.TAG_CLOSE_START);
			inside_tag		= true;
			update_matchers(tag_close_start_matcher.end());
			return token;
		}

		if (tag_start_matcher.lookingAt() && tag_start_matcher.start() == 0)
		{
			XMLToken	token	= new XMLToken(tag_start_matcher.group(), XMLToken.TAG_START);
			inside_tag		= true;
			update_matchers(tag_start_matcher.end());
			return token;
		}

		if (tag_end_matcher.lookingAt() && tag_end_matcher.start() == 0)
		{
			XMLToken	token	= new XMLToken(tag_end_matcher.group(), XMLToken.TAG_END);
			inside_tag		= false;
			update_matchers(tag_end_matcher.end());
			return token;
		}

		if (null_tag_end_matcher.lookingAt() && null_tag_end_matcher.start() == 0)
		{
			XMLToken	token	= new XMLToken(null_tag_end_matcher.group(), XMLToken.NULL_TAG_END);
			inside_tag		= false;
			update_matchers(null_tag_end_matcher.end());
			return token;
		}

		if (inside_tag)
		{
			if (attribute_matcher.lookingAt() && attribute_matcher.start() == 0)
			{
				XMLToken	token	= new XMLToken(attribute_matcher.group(), XMLToken.ATTRIBUTE);
				update_matchers(attribute_matcher.end());
				return token;
			}

			if (attribute_value_matcher.lookingAt() && attribute_value_matcher.start() == 0)
			{
				XMLToken	token	= new XMLToken(attribute_value_matcher.group(), XMLToken.ATTRIBUTE_VALUE);
				update_matchers(attribute_value_matcher.end());
				return token;
			}

			if (prolog_identifier_matcher.lookingAt() && prolog_identifier_matcher.start() == 0)
			{
				XMLToken	token = new XMLToken(prolog_identifier_matcher.group(), XMLToken.PROLOG_IDENTIFIER);
				update_matchers(prolog_identifier_matcher.end());
				return token;
			}

			if (element_matcher.lookingAt() && element_matcher.start() == 0)
			{
				XMLToken	token	= new XMLToken(element_matcher.group(), XMLToken.ELEMENT);
				update_matchers(element_matcher.end());
				return token;
			}
		}
		else if (value_matcher.lookingAt() && value_matcher.start() == 0)
		{
			XMLToken	token	= new XMLToken(value_matcher.group(), XMLToken.VALUE);
			update_matchers(value_matcher.end());
			return token;
		}

		if (space_to_eol_matcher.lookingAt() && space_to_eol_matcher.start() == 0)
		{
			update_matchers(space_to_eol_matcher.end());
			return getNextToken();
		}

		return new XMLToken("", XMLToken.NULL);
	}

	private void update_matchers(int index)
	{
		if (index != 0)
		{
			if (index >= line.length())
			{
				line	= null;
				return;
			}

			this.index += index;

			line	= line.substring(index);
		}

		prolog_start_matcher		= prolog_start_matcher.reset(line);
		prolog_identifier_matcher	= prolog_identifier_matcher.reset(line);
		attribute_value_matcher		= attribute_value_matcher.reset(line);
		prolog_end_matcher		= prolog_end_matcher.reset(line);
		tag_start_matcher		= tag_start_matcher.reset(line);
		element_matcher			= element_matcher.reset(line);
		attribute_matcher		= attribute_matcher.reset(line);
		null_tag_end_matcher		= null_tag_end_matcher.reset(line);
		tag_close_start_matcher		= tag_close_start_matcher.reset(line);
		value_matcher			= value_matcher.reset(line);
		tag_end_matcher			= tag_end_matcher.reset(line);
		space_to_eol_matcher		= space_to_eol_matcher.reset(line);
	}

	public static void main(String args[])
	{
		for (int i = 0; i < args.length; i++)
		{
			XMLTokenizer		tokenizer	= null;

			try
			{
				tokenizer	= new XMLTokenizer(args[i]);
			}
			catch (java.io.FileNotFoundException e)
			{
				System.out.println("Unable to read file '" + args[i] + "'");
				continue;
			}

			XMLTokenizer.XMLToken	token		= null;

			System.out.println("File:  '" + args[i] + "'");

			do
			{
				try
				{
					token	= tokenizer.getNextToken();
				}
				catch (java.io.IOException e)
				{
					System.out.println("IO Exception parsing file '" + args[i] + "':  " + e);
					e.printStackTrace();
				}

				System.out.println("\tLine " + tokenizer.getLineNumber() + ":  " +
				  token + " = '" + (token.getToken() == null ? "" : token.getToken()) + "'");
			} while (token.getTokenType() != XMLToken.NULL);
		}
	}
}
