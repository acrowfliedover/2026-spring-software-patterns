#include "DOMDocumentAdapter.hpp"
#include "Element.H"
#include "Text.H"
#include "Attr.H"

namespace XERCES
{

DOMDocumentAdapter::DOMDocumentAdapter(dom::Document* d)
    : DOMNodeAdapterBase<DOMDocument>(d), doc_(d)
{
}

DOMDocumentAdapter::~DOMDocumentAdapter() {}

// ---------------------------------------------------------------------------
//  Private helpers – wrap a shared_ptr from the adaptee in a Xerces adapter
//  and keep both alive in the document's owned-object lists.
// ---------------------------------------------------------------------------

DOMElementAdapter* DOMDocumentAdapter::wrapElement(std::shared_ptr<dom::Element> elem) const
{
    if (!elem) return nullptr;
    ownedDomNodes_.push_back(elem);
    DOMElementAdapter* a = new DOMElementAdapter(elem.get());
    ownedAdapters_.push_back(std::unique_ptr<DOMNode>(a));
    return a;
}

DOMTextAdapter* DOMDocumentAdapter::wrapText(std::shared_ptr<dom::Text> text) const
{
    if (!text) return nullptr;
    ownedDomNodes_.push_back(text);
    DOMTextAdapter* a = new DOMTextAdapter(text.get());
    ownedAdapters_.push_back(std::unique_ptr<DOMNode>(a));
    return a;
}

DOMAttrAdapter* DOMDocumentAdapter::wrapAttr(std::shared_ptr<dom::Attr> attr) const
{
    if (!attr) return nullptr;
    ownedDomNodes_.push_back(attr);
    DOMAttrAdapter* a = new DOMAttrAdapter(attr.get());
    ownedAdapters_.push_back(std::unique_ptr<DOMNode>(a));
    return a;
}

// ---------------------------------------------------------------------------
//  Delegated DOMDocument methods
// ---------------------------------------------------------------------------

DOMElement* DOMDocumentAdapter::createElement(const XMLCh* tagName)
{
    return wrapElement(doc_->createElement(fromXMLString(tagName)));
}

DOMText* DOMDocumentAdapter::createTextNode(const XMLCh* data)
{
    return wrapText(doc_->createTextNode(fromXMLString(data)));
}

DOMAttr* DOMDocumentAdapter::createAttribute(const XMLCh* name)
{
    return wrapAttr(doc_->createAttribute(fromXMLString(name)));
}

DOMElement* DOMDocumentAdapter::getDocumentElement() const
{
    return wrapElement(doc_->getDocumentElement());
}

}; // namespace XERCES
