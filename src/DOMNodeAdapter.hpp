#ifndef DOM_NODE_ADAPTER_HPP
#define DOM_NODE_ADAPTER_HPP

#include "XercesStubs.hpp"
#include "XercesConvert.hpp"
#include "Node.H"

namespace XERCES
{

// Maps dom::Node type constants to XERCES::DOMNode::NodeType.
inline DOMNode::NodeType mapNodeType(short domType)
{
    switch (domType)
    {
        case dom::Node::ELEMENT_NODE:   return DOMNode::ELEMENT_NODE;
        case dom::Node::ATTRIBUTE_NODE: return DOMNode::ATTRIBUTE_NODE;
        case dom::Node::TEXT_NODE:      return DOMNode::TEXT_NODE;
        case dom::Node::DOCUMENT_NODE:  return DOMNode::DOCUMENT_NODE;
        default:                        return DOMNode::ELEMENT_NODE;
    }
}

// ---------------------------------------------------------------------------
//  DOMNodeAdapterBase<XercesBase>
//
//  Template that implements every XERCES::DOMNode pure virtual by
//  delegating to a dom::Node* adaptee (composition).  Methods the
//  project DOM does not support are stubbed with null / empty / defaults.
//
//  XercesBase is one of: DOMNode, DOMElement, DOMText, DOMAttr, DOMDocument.
// ---------------------------------------------------------------------------

template <typename XercesBase>
class DOMNodeAdapterBase : public XercesBase
{
protected:
    dom::Node* adaptee_;

public:
    explicit DOMNodeAdapterBase(dom::Node* n) : adaptee_(n) {}
    virtual ~DOMNodeAdapterBase() {}

    dom::Node* getAdaptee() const { return adaptee_; }

    // ---- Delegated to dom::Node -----------------------------------------

    const XMLCh* getNodeName() const override
    {
        return adaptee_->getNodeName().c_str();
    }

    const XMLCh* getNodeValue() const override
    {
        return adaptee_->getNodeValue().c_str();
    }

    DOMNode::NodeType getNodeType() const override
    {
        return mapNodeType(adaptee_->getNodeType());
    }

    void setNodeValue(const XMLCh* nodeValue) override
    {
        adaptee_->setNodeValue(fromXMLString(nodeValue));
    }

    bool hasChildNodes() const override
    {
        return adaptee_->hasChildNodes();
    }

    const XMLCh* getLocalName() const override
    {
        return adaptee_->getLocalName().c_str();
    }

    // ---- Stubbed (not supported by project DOM) -------------------------

    DOMNode*         getParentNode() const override      { return nullptr; }
    DOMNodeList*     getChildNodes() const override      { return nullptr; }
    DOMNode*         getFirstChild() const override      { return nullptr; }
    DOMNode*         getLastChild() const override       { return nullptr; }
    DOMNode*         getPreviousSibling() const override { return nullptr; }
    DOMNode*         getNextSibling() const override     { return nullptr; }
    DOMNamedNodeMap* getAttributes() const override      { return nullptr; }
    DOMDocument*     getOwnerDocument() const override   { return nullptr; }

    DOMNode* cloneNode(bool) const override                { return nullptr; }
    DOMNode* insertBefore(DOMNode*, DOMNode*) override     { return nullptr; }
    DOMNode* replaceChild(DOMNode*, DOMNode*) override     { return nullptr; }
    DOMNode* removeChild(DOMNode*) override                { return nullptr; }
    DOMNode* appendChild(DOMNode*) override                { return nullptr; }

    void normalize() override {}

    bool isSupported(const XMLCh*, const XMLCh*) const override { return false; }

    const XMLCh* getNamespaceURI() const override { return nullptr; }
    const XMLCh* getPrefix() const override       { return nullptr; }
    void         setPrefix(const XMLCh*) override {}

    bool hasAttributes() const override { return false; }

    bool isSameNode(const DOMNode* other) const override
    {
        return this == other;
    }

    bool isEqualNode(const DOMNode*) const override { return false; }

    void* setUserData(const XMLCh*, void*, DOMUserDataHandler*) override
    {
        return nullptr;
    }

    void* getUserData(const XMLCh*) const override { return nullptr; }

    const XMLCh* getBaseURI() const override { return nullptr; }

    short compareDocumentPosition(const DOMNode*) const override { return 0; }

    const XMLCh* getTextContent() const override   { return nullptr; }
    void         setTextContent(const XMLCh*) override {}

    const XMLCh* lookupPrefix(const XMLCh*) const override       { return nullptr; }
    bool         isDefaultNamespace(const XMLCh*) const override { return false; }
    const XMLCh* lookupNamespaceURI(const XMLCh*) const override { return nullptr; }

    void* getFeature(const XMLCh*, const XMLCh*) const override { return nullptr; }

    void release() override {}
};

// ---------------------------------------------------------------------------
//  Concrete adapters
// ---------------------------------------------------------------------------

class DOMNodeAdapter : public DOMNodeAdapterBase<DOMNode>
{
public:
    explicit DOMNodeAdapter(dom::Node* n)
        : DOMNodeAdapterBase<DOMNode>(n) {}
};

class DOMElementAdapter : public DOMNodeAdapterBase<DOMElement>
{
public:
    explicit DOMElementAdapter(dom::Node* n)
        : DOMNodeAdapterBase<DOMElement>(n) {}
};

class DOMTextAdapter : public DOMNodeAdapterBase<DOMText>
{
public:
    explicit DOMTextAdapter(dom::Node* n)
        : DOMNodeAdapterBase<DOMText>(n) {}
};

class DOMAttrAdapter : public DOMNodeAdapterBase<DOMAttr>
{
public:
    explicit DOMAttrAdapter(dom::Node* n)
        : DOMNodeAdapterBase<DOMAttr>(n) {}
};

}; // namespace XERCES

#endif // DOM_NODE_ADAPTER_HPP
