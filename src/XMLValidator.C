#include <cstdio>

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

XMLValidator::Memento XMLValidator::createMemento(void)
{
	Memento	m;

	for (std::vector<std::shared_ptr<ValidChildren>>::iterator i = schema.begin(); i != schema.end(); i++)
		m.state.push_back(std::make_shared<ValidChildren>(**i));

	return m;
}

void XMLValidator::setMemento(const Memento & m)
{
	schema.clear();

	for (std::vector<std::shared_ptr<ValidChildren>>::const_iterator i = m.state.begin(); i != m.state.end(); i++)
		schema.push_back(std::make_shared<ValidChildren>(**i));
}

void XMLValidator::printSchema(void) const
{
	for (std::vector<std::shared_ptr<ValidChildren>>::const_iterator i = schema.begin(); i != schema.end(); i++)
	{
		const std::shared_ptr<ValidChildren> &	vc	= *i;

		printf("  element '%s'  canHaveText=%d\n", vc->getThisElement().c_str(), vc->canHaveText() ? 1 : 0);

		const std::vector<std::string> &	children	= vc->getValidChildren();
		const std::vector<bool> &		isAttr		= vc->getChildIsAttribute();

		for (size_t j = 0; j < children.size(); j++)
			printf("    valid child '%s' (%s)\n", children[j].c_str(), isAttr[j] ? "attribute" : "element");
	}
}
