#ifndef XERCES_STUBS_HPP
#define XERCES_STUBS_HPP

#include "DOMNode.hpp"
#include "DOMDocument.hpp"

namespace XERCES
{

// ---------------------------------------------------------------------------
//  Types referenced by DOMNode (forward-declared in DOMNode.hpp)
// ---------------------------------------------------------------------------

class DOMUserDataHandler
{
public:
    virtual ~DOMUserDataHandler() {}
};

class DOMNodeList
{
public:
    virtual ~DOMNodeList() {}
    virtual DOMNode*     item(unsigned int) const { return nullptr; }
    virtual unsigned int getLength() const        { return 0; }
};

class DOMNamedNodeMap
{
public:
    virtual ~DOMNamedNodeMap() {}
    virtual DOMNode*     getNamedItem(const XMLCh*) const { return nullptr; }
    virtual DOMNode*     setNamedItem(DOMNode*)           { return nullptr; }
    virtual DOMNode*     removeNamedItem(const XMLCh*)    { return nullptr; }
    virtual unsigned int getLength() const                { return 0; }
    virtual DOMNode*     item(unsigned int) const         { return nullptr; }
};

// ---------------------------------------------------------------------------
//  Types referenced by DOMDocument (forward-declared in DOMDocument.hpp)
// ---------------------------------------------------------------------------

class DOMDocumentType
{
public:
    virtual ~DOMDocumentType() {}
};

class DOMImplementation
{
public:
    virtual ~DOMImplementation() {}
};

class DOMConfiguration
{
public:
    virtual ~DOMConfiguration() {}
};

class DOMEntity
{
public:
    virtual ~DOMEntity() {}
};

class DOMNotation
{
public:
    virtual ~DOMNotation() {}
};

class DOMNodeFilter
{
public:
    virtual ~DOMNodeFilter() {}
};

// ---------------------------------------------------------------------------
//  DOMNode subtypes -- abstract (inherit DOMNode pure virtuals).
//  Only used as pointer return types; the adapters provide concrete versions.
// ---------------------------------------------------------------------------

class DOMElement : public DOMNode
{
protected:
    DOMElement() {}
public:
    virtual ~DOMElement() {}
};

class DOMText : public DOMNode
{
protected:
    DOMText() {}
public:
    virtual ~DOMText() {}
};

class DOMAttr : public DOMNode
{
protected:
    DOMAttr() {}
public:
    virtual ~DOMAttr() {}
};

class DOMDocumentFragment : public DOMNode
{
protected:
    DOMDocumentFragment() {}
public:
    virtual ~DOMDocumentFragment() {}
};

class DOMComment : public DOMNode
{
protected:
    DOMComment() {}
public:
    virtual ~DOMComment() {}
};

class DOMCDATASection : public DOMNode
{
protected:
    DOMCDATASection() {}
public:
    virtual ~DOMCDATASection() {}
};

class DOMProcessingInstruction : public DOMNode
{
protected:
    DOMProcessingInstruction() {}
public:
    virtual ~DOMProcessingInstruction() {}
};

class DOMEntityReference : public DOMNode
{
protected:
    DOMEntityReference() {}
public:
    virtual ~DOMEntityReference() {}
};

}; // namespace XERCES

#endif // XERCES_STUBS_HPP
