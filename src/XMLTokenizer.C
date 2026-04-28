#include "XMLTokenizer.H"

const char *	XMLTokenizer::PROLOG_START	= "<\\?";
const char *	XMLTokenizer::PROLOG_IDENTIFIER	= "[[:space:]]*xml";
const char *	XMLTokenizer::ATTRIBUTE_VALUE	= "\"[^\"]*\"";
const char *	XMLTokenizer::PROLOG_END	= "[[:space:]]*\\?>";
const char *	XMLTokenizer::TAG_START		= "[[:space:]]*<";
const char *	XMLTokenizer::ELEMENT		= "[[:space:]]*[[:alpha:]]([[:alnum:]_-]|:)*";
const char *	XMLTokenizer::ATTRIBUTE		= "[[:space:]]+[[:alpha:]]([[:alnum:]_-]|:)*[[:space:]]*=";
const char *	XMLTokenizer::NULL_TAG_END	= "[[:space:]]*/>";
const char *	XMLTokenizer::TAG_CLOSE_START	= "[[:space:]]*</";
const char *	XMLTokenizer::VALUE		= "[^<]*";
const char *	XMLTokenizer::TAG_END		= "[[:space:]]*>";
const char *	XMLTokenizer::SPACE_TO_EOL	= "[[:space:]]*";

XMLTokenizer::XMLToken::XMLToken(const std::string & t, TokenTypes tt) : token(t), token_type(tt) {}

const char *	XMLTokenizer::XMLToken::toString(void)
{
	switch(token_type)
	{
	case NULL_TOKEN:
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
		return "NULL";
	}
}

XMLTokenizer::XMLTokenizer(const std::string & filename) :
  file(filename.c_str(), std::ios_base::in),
  line_number(0),
  index(0),
  inside_tag(false),
  pending_attribute_value(false),
  tag_found(false),
  prolog_start(PROLOG_START),
  prolog_identifier(PROLOG_IDENTIFIER),
  attribute_value(ATTRIBUTE_VALUE),
  prolog_end(PROLOG_END),
  tag_start(TAG_START),
  element(ELEMENT),
  attribute(ATTRIBUTE),
  null_tag_end(NULL_TAG_END),
  tag_close_start(TAG_CLOSE_START),
  value(VALUE),
  tag_end(TAG_END),
  space_to_eol(SPACE_TO_EOL)
{
}

std::shared_ptr<XMLTokenizer::XMLToken>	XMLTokenizer::getNextToken(void)
{
	if (line.size() == 0)
	{
		std::getline(file, line);
		index	= 0;
		line_number++;

		if (line.size() == 0)
			return std::make_shared<XMLToken>(std::string(""), XMLToken::NULL_TOKEN);
	}

	std::smatch	what;

	if (inside_tag)
	{
		if (!tag_found && std::regex_search(line, what, prolog_identifier, std::regex_constants::match_continuous))
		{
			std::shared_ptr<XMLToken>
			  token(new XMLToken(std::string(what[0].first, what[0].second), XMLToken::PROLOG_IDENTIFIER));

			update_matchers(what[0], what.suffix());
			tag_found	= true;
			return	token;
		}

		if (!tag_found && std::regex_search(line, what, element, std::regex_constants::match_continuous))
		{
			std::shared_ptr<XMLToken>
			  token(new XMLToken(std::string(what[0].first, what[0].second), XMLToken::ELEMENT));

			update_matchers(what[0], what.suffix());
			tag_found	= true;
			return	token;
		}

		if (pending_attribute_value && std::regex_search(line, what, attribute_value, std::regex_constants::match_continuous))
		{
			std::shared_ptr<XMLToken>
			  token(new XMLToken(std::string(what[0].first, what[0].second), XMLToken::ATTRIBUTE_VALUE));

			update_matchers(what[0], what.suffix());
			pending_attribute_value	= false;
			return	token;
		}

		if (std::regex_search(line, what, attribute, std::regex_constants::match_continuous))
		{
			std::shared_ptr<XMLToken>
			  token(new XMLToken(std::string(what[0].first, what[0].second), XMLToken::ATTRIBUTE));

			update_matchers(what[0], what.suffix());
			pending_attribute_value	= true;
			return	token;
		}

		if (std::regex_search(line, what, null_tag_end, std::regex_constants::match_continuous))
		{
			std::shared_ptr<XMLToken>
			  token(new XMLToken(std::string(what[0].first, what[0].second), XMLToken::NULL_TAG_END));

			inside_tag		= false;
			pending_attribute_value	= false;
			tag_found		= false;
			update_matchers(what[0], what.suffix());
			return	token;
		}

		if (std::regex_search(line, what, tag_end, std::regex_constants::match_continuous))
		{
			std::shared_ptr<XMLToken>
			  token(new XMLToken(std::string(what[0].first, what[0].second), XMLToken::TAG_END));

			inside_tag		= false;
			pending_attribute_value	= false;
			tag_found		= false;
			update_matchers(what[0], what.suffix());
			return	token;
		}

	}

	if (std::regex_search(line, what, prolog_start, std::regex_constants::match_continuous))
	{
		std::shared_ptr<XMLToken>
		  token(new XMLToken(std::string(what[0].first, what[0].second), XMLToken::PROLOG_START));

		inside_tag		= true;
		update_matchers(what[0], what.suffix());
		return	token;
	}

	if (std::regex_search(line, what, prolog_end, std::regex_constants::match_continuous))
	{
		std::shared_ptr<XMLToken>
		  token(new XMLToken(std::string(what[0].first, what[0].second), XMLToken::PROLOG_END));

		inside_tag		= false;
		pending_attribute_value	= false;
		tag_found		= false;
		update_matchers(what[0], what.suffix());
		return	token;
	}

	if (std::regex_search(line, what, tag_close_start, std::regex_constants::match_continuous))
	{
		std::shared_ptr<XMLToken>
		  token(new XMLToken(std::string(what[0].first, what[0].second), XMLToken::TAG_CLOSE_START));

		inside_tag		= true;
		update_matchers(what[0], what.suffix());
		return	token;
	}

	if (std::regex_search(line, what, tag_start, std::regex_constants::match_continuous))
	{
		std::shared_ptr<XMLToken>
		  token(new XMLToken(std::string(what[0].first, what[0].second), XMLToken::TAG_START));

		inside_tag		= true;
		update_matchers(what[0], what.suffix());
		return	token;
	}
	if (std::regex_search(line, what, value, std::regex_constants::match_continuous))
	{
		std::shared_ptr<XMLToken>	token(new XMLToken(std::string(what[0].first, what[0].second), XMLToken::VALUE));
		update_matchers(what[0], what.suffix());
		return	token;
	}

	if (std::regex_search(line, what, space_to_eol, std::regex_constants::match_continuous))
	{
		update_matchers(what[0], what.suffix());
		return	getNextToken();
	}

	return std::make_shared<XMLToken>(std::string(""), XMLToken::NULL_TOKEN);
}

void		XMLTokenizer::update_matchers(const std::ssub_match & matcher, const std::ssub_match & suffix)
{
	int		matcher_size	= matcher.str().size();
	std::string	remaining	= suffix.str();

	if (matcher_size >= line.size())
	{
		line.clear();
		return;
	}

	line	= remaining;
	index	+= matcher_size;
}
