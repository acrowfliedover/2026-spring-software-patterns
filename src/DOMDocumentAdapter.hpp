#ifndef DOM_DOCUMENT_ADAPTER_HPP
#define DOM_DOCUMENT_ADAPTER_HPP

#include "DOMNodeAdapter.hpp"
#include "Document.H"
#include <vector>
#include <memory>

namespace XERCES
{

// ---------------------------------------------------------------------------
//  DOMDocumentAdapter
//
//  Implements XERCES::DOMDocument (which extends DOMNode) by holding a
//  pointer to the project's dom::Document (composition).  Supported
//  methods delegate to the adaptee with std::string <-> XMLCh* conversion.
//  Unsupported methods return null / empty / defaults.
// ---------------------------------------------------------------------------

class DOMDocumentAdapter : public DOMNodeAdapterBase<DOMDocument>
{
private:
    dom::Document* doc_;

    mutable std::vector<std::shared_ptr<dom::Node>>  ownedDomNodes_;
    mutable std::vector<std::unique_ptr<DOMNode>>    ownedAdapters_;

    DOMElementAdapter* wrapElement(std::shared_ptr<dom::Element> elem) const;
    DOMTextAdapter*    wrapText(std::shared_ptr<dom::Text> text) const;
    DOMAttrAdapter*    wrapAttr(std::shared_ptr<dom::Attr> attr) const;

public:
    explicit DOMDocumentAdapter(dom::Document* d);
    virtual ~DOMDocumentAdapter();

    // -- DOMNode overrides specific to Document nodes ---------------------

    DOMNode::NodeType getNodeType() const override { return DOMNode::DOCUMENT_NODE; }
    DOMDocument*      getOwnerDocument() const override { return nullptr; }

    // -- Delegated DOMDocument methods ------------------------------------

    DOMElement* createElement(const XMLCh* tagName) override;
    DOMText*    createTextNode(const XMLCh* data) override;
    DOMAttr*    createAttribute(const XMLCh* name) override;
    DOMElement* getDocumentElement() const override;

    // -- Stubbed DOMDocument methods --------------------------------------

    DOMDocumentFragment*       createDocumentFragment() override           { return nullptr; }
    DOMComment*                createComment(const XMLCh*) override        { return nullptr; }
    DOMCDATASection*           createCDATASection(const XMLCh*) override   { return nullptr; }
    DOMProcessingInstruction*  createProcessingInstruction(const XMLCh*,
                                   const XMLCh*) override                  { return nullptr; }
    DOMEntityReference*        createEntityReference(const XMLCh*) override{ return nullptr; }
    DOMDocumentType*           getDoctype() const override                 { return nullptr; }
    DOMImplementation*         getImplementation() const override          { return nullptr; }
    DOMNodeList*               getElementsByTagName(const XMLCh*) const override
                                                                           { return nullptr; }

    DOMNode*    importNode(const DOMNode*, bool) override                  { return nullptr; }
    DOMElement* createElementNS(const XMLCh*, const XMLCh*) override       { return nullptr; }
    DOMAttr*    createAttributeNS(const XMLCh*, const XMLCh*) override     { return nullptr; }
    DOMNodeList* getElementsByTagNameNS(const XMLCh*,
                                        const XMLCh*) const override       { return nullptr; }
    DOMElement* getElementById(const XMLCh*) const override                { return nullptr; }

    const XMLCh* getInputEncoding() const override     { return nullptr; }
    const XMLCh* getXmlEncoding() const override       { return nullptr; }
    bool         getXmlStandalone() const override     { return false; }
    void         setXmlStandalone(bool) override       {}
    const XMLCh* getXmlVersion() const override        { return nullptr; }
    void         setXmlVersion(const XMLCh*) override  {}
    const XMLCh* getDocumentURI() const override       { return nullptr; }
    void         setDocumentURI(const XMLCh*) override {}
    bool         getStrictErrorChecking() const override { return false; }
    void         setStrictErrorChecking(bool) override   {}

    DOMNode*          renameNode(DOMNode*, const XMLCh*, const XMLCh*) override { return nullptr; }
    DOMNode*          adoptNode(DOMNode*) override                              { return nullptr; }
    void              normalizeDocument() override                              {}
    DOMConfiguration* getDOMConfig() const override                             { return nullptr; }

    DOMEntity*        createEntity(const XMLCh*) override                       { return nullptr; }
    DOMDocumentType*  createDocumentType(const XMLCh*) override                 { return nullptr; }
    DOMNotation*      createNotation(const XMLCh*) override                     { return nullptr; }
    DOMElement*       createElementNS(const XMLCh*, const XMLCh*,
                          const XMLFileLoc, const XMLFileLoc) override          { return nullptr; }
};

}; // namespace XERCES

#endif // DOM_DOCUMENT_ADAPTER_HPP
