#include <string>
#include <vector>
#include <memory>

#include "Element.H"

class XMLValidator
{
public:
	class Memento
	{
		friend class XMLValidator;
	private:
		std::vector<std::shared_ptr<ValidChildren>>	state;
	public:
		Memento(void)	{}
		const std::vector<std::shared_ptr<ValidChildren>> & getState(void) const	{ return state; }
		void setState(const std::vector<std::shared_ptr<ValidChildren>> & s)		{ state = s; }
	};

private:
	std::vector<std::shared_ptr<ValidChildren>>	schema;
public:
	XMLValidator(void)	{}
	~XMLValidator() 	{}

	virtual std::shared_ptr<ValidChildren>				addSchemaElement(std::string element);
	virtual std::vector<std::shared_ptr<ValidChildren>>::iterator	findSchemaElement(std::string element);

	virtual Memento	createMemento(void);
	virtual void		setMemento(const Memento & m);
	virtual void		printSchema(void) const;
};
