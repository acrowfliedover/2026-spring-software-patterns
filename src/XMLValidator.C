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

std::shared_ptr<Memento> XMLValidator::CreateMemento(void)
{
	return std::shared_ptr<Memento>(new Memento_Impl(schema));
}

bool XMLValidator::SetMemento(std::shared_ptr<Memento> memento)
{
	if (std::dynamic_pointer_cast<Memento_Impl>(memento))
	{
		std::shared_ptr<Memento_Impl>	m(std::dynamic_pointer_cast<Memento_Impl>(memento));

		m->GetSchema(schema);

		return true;
	}
	else
		return false;
}

Memento_Impl::Memento_Impl(std::vector<std::shared_ptr<ValidChildren>> & _schema)
{
	duplicateSchema(_schema, schema);
}

void Memento_Impl::GetSchema(std::vector<std::shared_ptr<ValidChildren>> & s)
{
	duplicateSchema(schema, s);
}

void Memento_Impl::duplicateSchema(std::vector<std::shared_ptr<ValidChildren>> & ins,
  std::vector<std::shared_ptr<ValidChildren>> & outs)
{
	outs.clear();

	for (std::vector<std::shared_ptr<ValidChildren>>::iterator iterator = ins.begin(); iterator != ins.end(); iterator++)
		outs.push_back(std::make_shared<ValidChildren>(**iterator));
}
