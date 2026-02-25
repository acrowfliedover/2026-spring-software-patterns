#include <string>
#include <fstream>
#include <ios>
#include <vector>
#include <memory>

#include "Element.H"

class ValidChildren;

class XMLValidator
{
private:
	std::vector<std::shared_ptr<ValidChildren>>	schema;
public:
	XMLValidator(void)	{}
	~XMLValidator() 	{}

	virtual std::shared_ptr<ValidChildren>				addSchemaElement(std::string element);
	virtual std::vector<std::shared_ptr<ValidChildren>>::iterator	findSchemaElement(std::string element);
};
