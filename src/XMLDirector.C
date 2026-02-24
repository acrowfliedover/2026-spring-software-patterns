#include "XMLDirector.H"

XMLDirector::XMLDirector(AbstractXMLBuilder *b) : builder(b) {}

void XMLDirector::construct(XMLTokenizer &tokenizer)
{
	XMLTokenizer::XMLToken *token = 0;
	bool inProlog = false;
	bool closingTag = false;
	std::string pendingAttributeName;

	do
	{
		delete token;
		token = tokenizer.getNextToken();

		switch (token->getTokenType())
		{
		case XMLTokenizer::XMLToken::PROLOG_START:
			inProlog = true;
			break;

		case XMLTokenizer::XMLToken::PROLOG_END:
			inProlog = false;
			break;

		case XMLTokenizer::XMLToken::TAG_START:
			closingTag = false;
			break;

		case XMLTokenizer::XMLToken::TAG_CLOSE_START:
			closingTag = true;
			break;

		case XMLTokenizer::XMLToken::ELEMENT:
		{
			if (inProlog)
				break;

			std::string elementName = token->getToken();
			elementName.erase(0, elementName.find_first_not_of(" \t\n\r"));
			elementName.erase(elementName.find_last_not_of(" \t\n\r") + 1);

			if (closingTag)
			{
				builder->goToParent();
			}
			else
			{
				builder->addChild(elementName, "", dom::Node::ELEMENT_NODE);
				builder->goToChild(-1);
			}
			break;
		}

		case XMLTokenizer::XMLToken::ATTRIBUTE:
		{
			if (inProlog)
				break;

			pendingAttributeName = token->getToken();
			pendingAttributeName.erase(0, pendingAttributeName.find_first_not_of(" \t\n\r"));
			size_t eqPos = pendingAttributeName.find('=');
			if (eqPos != std::string::npos)
				pendingAttributeName = pendingAttributeName.substr(0, eqPos);
			pendingAttributeName.erase(pendingAttributeName.find_last_not_of(" \t\n\r") + 1);
			break;
		}

		case XMLTokenizer::XMLToken::ATTRIBUTE_VALUE:
		{
			if (inProlog)
				break;

			std::string attrValue = token->getToken();
			if (attrValue.size() >= 2 && attrValue.front() == '"' && attrValue.back() == '"')
				attrValue = attrValue.substr(1, attrValue.size() - 2);

			builder->addChild(pendingAttributeName, attrValue, dom::Node::ATTRIBUTE_NODE);
			pendingAttributeName.clear();
			break;
		}

		case XMLTokenizer::XMLToken::VALUE:
		{
			std::string textValue = token->getToken();
			textValue.erase(0, textValue.find_first_not_of(" \t\n\r"));
			textValue.erase(textValue.find_last_not_of(" \t\n\r") + 1);

			if (!textValue.empty())
				builder->addChild(textValue, "", dom::Node::TEXT_NODE);
			break;
		}

		case XMLTokenizer::XMLToken::NULL_TAG_END:
			if (!inProlog)
				builder->goToParent();
			break;

		case XMLTokenizer::XMLToken::TAG_END:
			if (inProlog)
				inProlog = false;
			break;

		case XMLTokenizer::XMLToken::NULL_TOKEN:
		case XMLTokenizer::XMLToken::PROLOG_IDENTIFIER:
			break;
		}
	} while (token->getTokenType() != XMLTokenizer::XMLToken::NULL_TOKEN);

	delete token;
}
