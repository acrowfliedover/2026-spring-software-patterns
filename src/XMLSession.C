#include "XMLSession.H"
#include "NodeList.H"

XMLSession::XMLSession(void) : doc(std::make_shared<Document_Impl>())
{
}

static std::shared_ptr<dom::Element>	walk(dom::NodeList * list, const std::string & tag)
{
	if (list == 0)
		return std::shared_ptr<dom::Element>();

	for (auto i = list->begin(); i != list->end(); i++)
	{
		std::shared_ptr<dom::Node>	child(*i);
		std::shared_ptr<dom::Element>	asElement(std::dynamic_pointer_cast<dom::Element>(child));

		if (asElement && asElement->getTagName() == tag)
			return asElement;

		std::shared_ptr<dom::Element>	hit(walk(child->getChildNodes(), tag));

		if (hit)
			return hit;
	}

	return std::shared_ptr<dom::Element>();
}

std::shared_ptr<dom::Element>	XMLSession::findElement(const std::string & tag)
{
	if (!doc)
		return std::shared_ptr<dom::Element>();

	return walk(doc->getChildNodes(), tag);
}
