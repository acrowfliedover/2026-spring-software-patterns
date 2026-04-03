#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include <string>
#include <memory>

namespace dom
{
	class Node;
};

class Observer;

class Subject
{
public:
	void	attach(std::shared_ptr<Observer> observer);
	void	detach(std::shared_ptr<Observer> observer);
protected:
	void notify(std::shared_ptr<dom::Node> container, short targetType, const std::string & target);
};

#endif // SUBJECT_H
