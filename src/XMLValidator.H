#include <string>
#include <fstream>
#include <ios>
#include <vector>
#include <memory>

#include "Element.H"

class ValidChildren;

class Memento
{
protected:
	Memento() {}
	virtual ~Memento() {}
};

class XMLValidator
{
private:
	std::vector<std::shared_ptr<ValidChildren>>	schema;
public:
	XMLValidator(void)	{}
	~XMLValidator() 	{}

	virtual std::shared_ptr<ValidChildren>				addSchemaElement(std::string element);
	virtual std::vector<std::shared_ptr<ValidChildren>>::iterator	findSchemaElement(std::string element);

	std::shared_ptr<Memento>	 				CreateMemento(void);
	bool								SetMemento(std::shared_ptr<Memento> memento);
};

class Memento_Impl : public Memento
{
	friend class XMLValidator;
private:
	std::vector<std::shared_ptr<ValidChildren>>	schema;

	Memento_Impl(std::vector<std::shared_ptr<ValidChildren>> & _schema);
	void		GetSchema(std::vector<std::shared_ptr<ValidChildren>> & s);
	void		duplicateSchema(std::vector<std::shared_ptr<ValidChildren>> & ins,
			  std::vector<std::shared_ptr<ValidChildren>> & outs);
};
