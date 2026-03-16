#ifndef XERCES_CONVERT_HPP
#define XERCES_CONVERT_HPP

#include <string>
#include "DOMNode.hpp"   // XERCES::XMLCh (typedef'd to char)

namespace XERCES
{

// XMLCh is char in our stubs, so conversions are trivial.

inline std::string fromXMLString(const XMLCh* xs)
{
    return xs ? std::string(xs) : std::string();
}

// Use this when you need a temporary XMLCh buffer whose lifetime
// you control (the returned string owns the memory).
inline std::string toXMLString(const std::string& s)
{
    return s;
}

}; // namespace XERCES

#endif // XERCES_CONVERT_HPP
