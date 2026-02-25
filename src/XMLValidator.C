#include "XMLValidator.H"
#include "ValidChildren.H"

std::shared_ptr<ValidChildren> XMLValidator::addSchemaElement(std::string element)
{
	std::vector<std::shared_ptr<ValidChildren>>::iterator	schemaElementIterator	= findSchemaElement(element);

	if (schemaElementIterator != schema.end())
	{
		schema.erase(schemaElementIterator);
	}

	std::shared_ptr<ValidChildren>	schemaElement	= 0;
	schema.push_back(schemaElement = std::make_shared<ValidChildren>(element));
	return schemaElement;
}

std::vector<std::shared_ptr<ValidChildren>>::iterator XMLValidator::findSchemaElement(std::string element)
{
	for (std::vector<std::shared_ptr<ValidChildren>>::iterator i = schema.begin(); i != schema.end(); i++)
		if (((*i)->getThisElement().size() == 0 && element.size() == 0) ||
		  ((*i)->getThisElement().size() > 0 && (*i)->getThisElement().compare(element) == 0))
			return i;

	return schema.end();
}
