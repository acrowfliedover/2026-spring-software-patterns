#ifndef PARSEOBSERVER_H
#define PARSEOBSERVER_H

#include <string>

struct ParseEvent
{
	enum EventType
	{
		PROLOG_START		= 0,
		PROLOG_END		= 1,
		PROLOG_ID		= 2,
		ELEMENT_CREATED		= 3,
		ATTRIBUTE_CREATED	= 4,
		ATTRIBUTE_VALUE		= 5,
		TEXT_ADDED		= 6,
		ELEMENT_PUSHED		= 7,
		ELEMENT_POPPED		= 8,
		ELEMENT_CONFIRMED	= 9
	};

	EventType	type;
	std::string	data;
	int		depth;

	ParseEvent(EventType _type, const std::string & _data, int _depth)
		: type(_type), data(_data), depth(_depth) {}

	ParseEvent(EventType _type, int _depth)
		: type(_type), depth(_depth) {}
};

class ParseObserver
{
public:
	virtual ~ParseObserver() {}
	virtual void update(const ParseEvent & event) = 0;
};

#endif // PARSEOBSERVER_H
