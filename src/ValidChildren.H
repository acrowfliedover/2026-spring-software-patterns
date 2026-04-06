#ifndef VALIDCHILDREN_H
#define VALIDCHILDREN_H

#include <vector>
#include <string>

class ValidChildren
{
private:
	std::string			thisElement;
	std::vector<std::string>	validChildren;
	std::vector<bool>		childIsAttribute;
	bool				_canHaveText;
public:
	ValidChildren(std::string _thisElement) : thisElement(_thisElement), _canHaveText(false)	{}

	virtual std::string	getThisElement(void)			{ return thisElement; }
	virtual bool		canHaveText(void)			{ return _canHaveText; }
	virtual void		setCanHaveText(bool __canHaveText)	{ _canHaveText = __canHaveText; }

	virtual const std::vector<std::string> &	getValidChildren(void) const		{ return validChildren; }
	virtual const std::vector<bool> &		getChildIsAttribute(void) const		{ return childIsAttribute; }

	virtual void		addValidChild(const std::string & child, bool isAttribute);
	virtual bool		childIsValid(const std::string & child, bool isAttribute);
};

#endif // VALIDCHILDREN_H
