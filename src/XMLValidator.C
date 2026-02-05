#include "XMLValidator.H"

ValidChildren * XMLValidator::addSchemaElement(std::string element)
{
	std::vector<ValidChildren *>::iterator	schemaElementIterator	= findSchemaElement(element);

	if (schemaElementIterator != schema.end())
	{
		schema.erase(schemaElementIterator);
		delete *schemaElementIterator;
	}

	ValidChildren *	schemaElement	= 0;
	schema.push_back(schemaElement = new ValidChildren(element));
	return schemaElement;
}

std::vector<ValidChildren *>::iterator XMLValidator::findSchemaElement(std::string element)
{
	for (std::vector<ValidChildren *>::iterator i = schema.begin(); i != schema.end(); i++)
		if (((*i)->getThisElement().size() == 0 && element.size() == 0) ||
		  ((*i)->getThisElement().size() > 0 && (*i)->getThisElement().compare(element) == 0))
			return i;

	return schema.end();
}

bool XMLValidator::canRootElement(std::string newElement)
{
	return canAddElement(0, newElement);
}

bool XMLValidator::canAddElement(dom::Element * element, std::string newElement)
{
	std::vector<ValidChildren *>::iterator	schemaElement	= findSchemaElement(element == 0 ? "" : element->getTagName());

	return schemaElement == schema.end() ? true : (*schemaElement)->childIsValid(newElement, false);
}

bool XMLValidator::canAddText(dom::Element * element)
{
	std::vector<ValidChildren *>::iterator	schemaElement	= findSchemaElement(element->getTagName());

	return schemaElement == schema.end() ? true : (*schemaElement)->canHaveText();
}

bool XMLValidator::canAddAttribute(dom::Element * element, std::string newAttribute)
{
	std::vector<ValidChildren *>::iterator	schemaElement	= findSchemaElement(element->getTagName());

	return schemaElement == schema.end() ? true : (*schemaElement)->childIsValid(newAttribute, true);
}

void ValidChildren::addValidChild(const std::string & child, bool isAttribute)
{
	if (childIsValid(child, isAttribute))
		return;

	validChildren.push_back(child);
	childIsAttribute.push_back(isAttribute);
}

bool ValidChildren::childIsValid(const std::string & child, bool isAttribute)
{
	for (int i = 0; i < validChildren.size(); i++)
		if (childIsAttribute[i] == isAttribute && validChildren[i].compare(child) == 0)
			return true;

	return false;
}
