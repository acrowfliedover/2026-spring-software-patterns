#ifndef NODELIST_H
#define NODELIST_H

#include <list>
#include <memory>

namespace dom
{
	class Node;

	class NodeList : public std::list<std::shared_ptr<Node>>
	{
	public:
		virtual iterator	find(Node * n)
		{
			iterator	i;

			for (i = begin(); i != end(); i++)
				if ((*i).get() == n)
					break;

			return i;
		}
	};
};

#endif // NODELIST_H
