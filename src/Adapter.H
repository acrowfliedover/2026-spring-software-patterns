//
// This is a partial implementation of the adaptation functionality showing the necessary wrapping and unwrapping functionality.
// Additional work is required to deal with shared_ptrs and memory safety.
//
#ifndef ADAPTER_H
#define ADAPTER_H

#include "Node.H"
#include "Document.H"
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMDocument.hpp>

namespace XERCES
{
	class DOMNodeList;
	class DOMNamedNodeMap;
	class DOMElement;
	class DOMText;
	class DOMAttr;
	class DOMDocumentFragment;
	class DOMComment;
	class DOMCDATASection;
	class DOMProcessingInstruction;
	class DOMEntityReference;
	class DOMDocumentType;
	class DOMImplementation;
	class DOMConfigutation;
	class DOMEntity;
	class DOMNotation;
};

namespace adapter
{
	class Node : public virtual XERCES::DOMNode
	{
	private:
		dom::Node *	adaptee;
	protected:
		Node(const std::string & name, short type) : adaptee(new Node_Impl(name, type))	{}
		Node(dom::Node * _adaptee) : adaptee(_adaptee)					{}
	public:
		virtual dom::Node *			getAdaptee()					{ return adaptee; }

		virtual const XERCES::XMLCh *		getNodeName() const;
		virtual const XERCES::XMLCh *		getNodeValue() const;
		virtual	XERCES::DOMNode::NodeType	getNodeType() const;
		virtual	XERCES::DOMNode *		getParentNode() const;
		virtual	XERCES::DOMNodeList *		getChildNodes() const;
		virtual	XERCES::DOMNode *		getFirstChild() const;
		virtual	XERCES::DOMNode *		getLastChild() const;
		virtual	XERCES::DOMNode *		getPreviousSibling() const;
		virtual	XERCES::DOMNode *		getNextSibling() const;
		virtual	XERCES::DOMDocument *		getOwnerDocument() const;
		virtual	XERCES::DOMNode *		insertBefore(XERCES::DOMNode * newChild, XERCES::DOMNode * refChild);
		virtual	XERCES::DOMNode *		replaceChild(XERCES::DOMNode * newChild, XERCES::DOMNode * oldChild);
		virtual	XERCES::DOMNode *		removeChild(XERCES::DOMNode * oldChild);
		virtual	XERCES::DOMNode *		appendChild(XERCES::DOMNode * newChild);
		virtual bool				hasChildNodes() const;
		virtual	void				setNodeValue(const XERCES::XMLCh * nodeValue);
		//
		// No implementation for these.
		//
		virtual	XERCES::DOMNamedNodeMap *	getAttributes() const				{ return 0; }
		virtual	XERCES::DOMNode *		cloneNode(bool deep) const			{ return 0; }
		virtual	void				normalize()					{}
		virtual bool	isSupported(const XERCES::XMLCh * feature, const XERCES::XMLCh * version) const	{ return false; }
		virtual const XERCES::XMLCh *		getNamespaceURI() const				{ return 0; }
		virtual const XERCES::XMLCh *		getPrefix() const				{ return 0; }
		virtual const XERCES::XMLCh *		getLocalName() const				{ return 0; }
		virtual	void				setPrefix(const XERCES::XMLCh * prefix)		{}
		virtual bool				hasAttributes() const				{ return false; }
		virtual bool				isSameNode(const XERCES::DOMNode * other) const	{ return false; }
		virtual bool				isEqualNode(const XERCES::DOMNode * arg) const	{ return false; }
		virtual	void *	setUserData(const XERCES::XMLCh * key, void * data, XERCES::DOMUserDataHandler * handler){return 0;}
		virtual	void *				getUserData(const XERCES::XMLCh * key) const	{ return 0; }
		virtual const XERCES::XMLCh *		getBaseURI() const				{ return 0; }
		virtual short				compareDocumentPosition(const XERCES::DOMNode * other) const { return 0; }
		virtual const XERCES::XMLCh *		getTextContent() const				{ return 0; }
		virtual	void				setTextContent(const XERCES::XMLCh * textContent)	{}
		virtual const XERCES::XMLCh *		lookupPrefix(const XERCES::XMLCh * namespaceURI) const	{ return 0; }
		virtual bool				isDefaultNamespace(const XERCES::XMLCh * namespaceURI) const {return false;}
		virtual const XERCES::XMLCh *		lookupNamespaceURI(const XERCES::XMLCh * prefix) const	{ return 0; }
		virtual	void *	getFeature(const XERCES::XMLCh * feature, const XERCES::XMLCh * version) const	{ return 0; }
		virtual	void				release()					{}
	};

	class Document : public virtual Node, public virtual XERCES::DOMDocument
	{
	public:
		Document(void) : Node(new Document_Impl())	{}
		Document(dom::Document * adaptee) : Node(adaptee)	{}

		virtual	XERCES::DOMElement *		createElement(const XERCES::XMLCh * tagName);
		virtual	XERCES::DOMText *		createTextNode(const XERCES::XMLCh * data);
		virtual	XERCES::DOMAttr *		createAttribute(const XERCES::XMLCh * name);
		virtual	XERCES::DOMElement *		getDocumentElement() const;
		//
		// These Node members must be reimplemented to correctly wrap the return result.
		//
		virtual	XERCES::DOMNode *		getParentNode() const;
		virtual	XERCES::DOMNode *		getFirstChild() const;
		virtual	XERCES::DOMNode *		getLastChild() const;
		virtual	XERCES::DOMNode *		getPreviousSibling() const;
		virtual	XERCES::DOMNode *		getNextSibling() const;
		virtual	XERCES::DOMNode *		insertBefore(XERCES::DOMNode * newChild, XERCES::DOMNode * refChild);
		virtual	XERCES::DOMNode *		replaceChild(XERCES::DOMNode * newChild, XERCES::DOMNode * oldChild);
		virtual	XERCES::DOMNode *		removeChild(XERCES::DOMNode * oldChild);
		virtual	XERCES::DOMNode *		appendChild(XERCES::DOMNode * newChild);

		virtual const XERCES::XMLCh *		getNodeName() const			{ return Node::getNodeName(); }
		virtual const XERCES::XMLCh *		getNodeValue() const			{ return Node::getNodeValue(); }
		virtual	XERCES::DOMNode::NodeType	getNodeType() const			{ return Node::getNodeType(); }
		virtual	XERCES::DOMNodeList *		getChildNodes() const			{ return Node::getChildNodes(); }
		virtual	XERCES::DOMDocument *		getOwnerDocument() const		{ return Node::getOwnerDocument(); }
		virtual bool				hasChildNodes() const			{ return Node::hasChildNodes(); }
		virtual	void				setNodeValue(const XERCES::XMLCh * nodeValue)
		  { Node::setNodeValue(nodeValue); }
		virtual	XERCES::DOMNamedNodeMap *	getAttributes() const				{ return 0; }
		virtual	XERCES::DOMNode *		cloneNode(bool deep) const			{ return 0; }
		virtual	void				normalize()					{}
		virtual bool	isSupported(const XERCES::XMLCh * feature, const XERCES::XMLCh * version) const	{ return false; }
		virtual const XERCES::XMLCh *		getNamespaceURI() const				{ return 0; }
		virtual const XERCES::XMLCh *		getPrefix() const				{ return 0; }
		virtual const XERCES::XMLCh *		getLocalName() const				{ return 0; }
		virtual	void				setPrefix(const XERCES::XMLCh * prefix)		{}
		virtual bool				hasAttributes() const				{ return false; }
		virtual bool				isSameNode(const XERCES::DOMNode * other) const	{ return false; }
		virtual bool				isEqualNode(const XERCES::DOMNode * arg) const	{ return false; }
		virtual	void *	setUserData(const XERCES::XMLCh * key, void * data, XERCES::DOMUserDataHandler * handler){return 0;}
		virtual	void *				getUserData(const XERCES::XMLCh * key) const	{ return 0; }
		virtual const XERCES::XMLCh *		getBaseURI() const				{ return 0; }
		virtual short				compareDocumentPosition(const XERCES::DOMNode * other) const { return 0; }
		virtual const XERCES::XMLCh *		getTextContent() const				{ return 0; }
		virtual	void				setTextContent(const XERCES::XMLCh * textContent)	{}
		virtual const XERCES::XMLCh *		lookupPrefix(const XERCES::XMLCh * namespaceURI) const	{ return 0; }
		virtual bool				isDefaultNamespace(const XERCES::XMLCh * namespaceURI) const {return false;}
		virtual const XERCES::XMLCh *		lookupNamespaceURI(const XERCES::XMLCh * prefix) const	{ return 0; }
		virtual	void *	getFeature(const XERCES::XMLCh * feature, const XERCES::XMLCh * version) const	{ return 0; }
		virtual	void				release()					{}
		//
		// No implementation for these.
		//
		virtual	XERCES::DOMDocumentFragment *	createDocumentFragment()				{ return 0; }
		virtual	XERCES::DOMComment *		createComment(const XERCES::XMLCh * data)		{ return 0; }
		virtual	XERCES::DOMCDATASection *	createCDATASection(const XERCES::XMLCh * data)		{ return 0; }
		virtual	XERCES::DOMProcessingInstruction *
		  createProcessingInstruction(const XERCES::XMLCh * adaptee, const XERCES::XMLCh * data)		{ return 0; }
		virtual	XERCES::DOMEntityReference *	createEntityReference(const XERCES::XMLCh * name)	{ return 0; }
		virtual	XERCES::DOMDocumentType *	getDoctype() const					{ return 0; }
		virtual	XERCES::DOMImplementation *	getImplementation() const				{ return 0; }
		virtual	XERCES::DOMNodeList *		getElementsByTagName(const XERCES::XMLCh * tagname) const { return 0; }
		virtual	XERCES::DOMNode *		importNode(const XERCES::DOMNode * importedNode, bool deep) { return 0; }
		virtual	XERCES::DOMElement *
		  createElementNS(const XERCES::XMLCh * namespaceURI, const XERCES::XMLCh * qualifiedName)	{ return 0; }
		virtual	XERCES::DOMAttr *
		  createAttributeNS(const XERCES::XMLCh * namespaceURI, const XERCES::XMLCh * qualifiedName)	{ return 0; }
		virtual	XERCES::DOMNodeList *
		  getElementsByTagNameNS(const XERCES::XMLCh * namespaceURI, const XERCES::XMLCh * localName) const { return 0; }
		virtual	XERCES::DOMElement *		getElementById(const XERCES::XMLCh * elementId) const	{ return 0; }
		virtual const XERCES::XMLCh *		getInputEncoding() const				{ return 0; }
		virtual const XERCES::XMLCh *		getXmlEncoding() const					{ return 0; }
		virtual bool				getXmlStandalone() const				{ return false; }
		virtual	void				setXmlStandalone(bool standalone)			{}
		virtual const XERCES::XMLCh *		getXmlVersion() const					{ return 0; }
		virtual	void				setXmlVersion(const XERCES::XMLCh * version)		{}
		virtual const XERCES::XMLCh *		getDocumentURI() const					{ return 0; }
		virtual	void				setDocumentURI(const XERCES::XMLCh * documentURI)	{}
		virtual bool				getStrictErrorChecking() const				{ return false; }
		virtual	void				setStrictErrorChecking(bool strictErrorChecking)	{}
		virtual	XERCES::DOMNode *
		  renameNode(XERCES::DOMNode * n, const XERCES::XMLCh * namespaceURI, const XERCES::XMLCh * qualifiedName)
		  { return 0; }
		virtual	XERCES::DOMNode *		adoptNode(XERCES::DOMNode * source)			{ return 0; }
		virtual	void				normalizeDocument()					{}
		virtual	XERCES::DOMConfiguration *	getDOMConfig() const					{ return 0; }
		virtual	XERCES::DOMEntity *		createEntity(const XERCES::XMLCh * name)		{ return 0; }
		virtual	XERCES::DOMDocumentType *	createDocumentType(const XERCES::XMLCh * name)		{ return 0; }
		virtual	XERCES::DOMNotation *		createNotation(const XERCES::XMLCh * name)		{ return 0; }
		virtual	XERCES::DOMElement *		createElementNS(const XERCES::XMLCh * namespaceURI,
		  const XERCES::XMLCh * qualifiedName, const XERCES::XMLFileLoc lineNum, const XERCES::XMLFileLoc columnNum)
		  { return 0; }
	};
};

#endif // ADAPTER_H
